#include "Server.hpp"
#include "defines.h"
/********************/
/* Command Includes */
/********************/

#include "commands/Away.hpp"
// #include "commands/Ban.hpp"
// #include "commands/Echo.hpp"
// #include "commands/Exit.hpp"
// #include "commands/Help.hpp"
// #include "commands/Info.hpp"
#include "commands/Join.hpp"
// #include "commands/Kick.hpp"
 #include "commands/List.hpp"
#include "commands/Mode.hpp"
// #include "commands/Names.hpp"
#include "commands/Nick.hpp"
#include "commands/Notice.hpp"
// #include "commands/Ope.hpp"
#include "commands/Part.hpp"
 #include "commands/Pass.hpp"
#include "commands/Ping.hpp"
#include "commands/Pong.hpp"
#include "commands/Privmsg.hpp"
#include "commands/Quit.hpp"
#include "commands/User.hpp"
#include "commands/Invite.hpp"
#include "Client.hpp"
#include "commands/Who.hpp"
#include "commands/Whois.hpp"
#include "commands/Topic.hpp"


/*****************************/
/* Constructor & Destructor */
/*****************************/

Server::Server(const std::string& hostname, const int port, const std::string& password) :
	_hostname(hostname), _password(password), _timeStart(std::time(nullptr)), _port(port) {
	
	try
	{
		/* Setup server connection */
		initializeConnection();
		std::cout << getTimestamp() << GREEN "Server initialization successful" CLEAR << std::endl;
		std::cout << "\t\t\t\tport: " << port << std::endl << "\t\t\t\tpass: " << password << std::endl;
		
		/* Initialize commands map */
		initializeCommands();
		std::cout << getTimestamp() << GREEN "Command initialization successful" CLEAR<< std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << RED "Failure to initialize server, program exiting" CLEAR << std::endl;
		exit (1);
	}

	/* Start Server Loop */
	std::cout << "_______________________________________________________" << std::endl << std::endl;
	std::cout << getTimestamp() <<  "Server status - " << GREEN "ONLINE" CLEAR << std::endl;
	std::cout << "_______________________________________________________" << std::endl << std::endl;
	runServer();
}

Server::~Server() {
	/* Delete Commands*/
	std::map<std::string, Command *>::iterator it = _commands.begin();
	for (; it != _commands.end(); ++it)
		delete (it->second);
	_commands.clear();

	/* Delete Clients */
	for (size_t i = 0; i < _clients.size(); i++)
		delete (_clients[i]);
	_clients.clear();
	
	/* Delete channels */
	std::map<std::string, Channel *>::iterator it_ch = _channels.begin();
	for (; it_ch != _channels.end(); it_ch++)
		delete (it_ch->second);
	_channels.clear();

	/* Close server socket */
	shutdown(_socket, SHUT_RDWR);
}

/***********************************/
/*      Server Initialization      */
/***********************************/

//FIXME: Need to ensure all exceptions are being caught
//FIXME: Check if we need to buffer outgoing communications (look into POLLOUT option for poll() )


/* Open server socket and configure for listening */
void	Server::initializeConnection(void)
{
	int	yes = 1;

	/* Create socket */
	if ((_socket  = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Unable to create socket");

	/* Set socket as non-blocking */
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	/* Set socket options to reuse addresses */
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw std::runtime_error("Unable to set socket options");

	/* Setup socket address struct */
	_address.sin_port = htons(_port);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	//FIXME: convert to using hints and proper method for address struct (confirm if this matters )

	/* Bind Socket */
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw std::runtime_error("Unable to bind socket");
	
	/* Set socket to passive listening */
	if (listen(_socket, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Unable to listen on socket");
	
	/* Set Server Status */
	_status = ONLINE;

	/* Set up pollFDs */
	pollfd pfd = {.fd = _socket, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);
}

/* Build Server Commands */
void	Server::initializeCommands(void)
{
	/* General Commands */
	_commands["ping"] = new Ping(this);
	_commands["pong"] = new Pong(this);
	// _commands["info"] = new Info(this);
	// _commands["exit"] = new Exit(this);
	// _commands["echo"] = new Echo(this);
	// _commands["help"] = new Help(this);
	// _commands["ban"] = new Ban(this);
	// _commands["ope"] = new Ope(this);
	_commands["quit"] = new Quit(this);
	_commands["mode"] = new Mode(this);
	_commands["away"] = new Away(this);

	/* Channel Commands */
	_commands["privmsg"] = new Privmsg(this);
	_commands["notice"] = new Notice(this);
	_commands["join"] = new Join(this);
	_commands["part"] = new Part(this);
	 _commands["list"] = new List(this);
	// _commands["names"] = new Names(this);
	// _commands["kick"] = new Kick(this);
	_commands["nick"] = new Nick(this);
	_commands["user"] = new User(this);
	_commands["pass"] = new Pass(this);
	// _commands["mode"] = new Mode(this);

	//_commands["who"] = new Who(this);
	//_commands["time"] = new Time(this);
	_commands["invite"] = new Invite(this);
	//_commands["motd"] = new Motd(this);
	_commands["who"] = new Who(this);
	_commands["whois"] = new Whois(this);
	_commands["topic"] = new Topic(this);


}


/****************************************/
/*      Server Operation Functions      */
/****************************************/

/* Manage Connection Requests from New Clients */
void	Server::handleConnections()
{

	std::cout << getTimestamp() << RED "Incoming connection request" CLEAR<< std::endl;

	int	new_fd;
	int	addressLen;
	struct sockaddr_in clientAddress;
	addressLen = sizeof(clientAddress);

	/* Attempt to connect to client and get client address info*/
	if ((new_fd = accept(_socket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen)) < 0)
		throw std::runtime_error("Failure to accept incoming connection due to socket error");
	
	/* Set socket option to ensure that we dont attempt to send on a socket that has been disconnected */
	int yes = 1;
	if (setsockopt(new_fd, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(int)) < 0)
		throw std::runtime_error("Unable to set socket options");

	/* Add client to _clients map and populate address variables */
	_clients.push_back(new Client(new_fd));
	_clients.back()->setAddress(clientAddress);
	_clients.back()->setHostname(inet_ntoa(clientAddress.sin_addr));

	/* Create new pollfd object and add to _pfds list */
	pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);

	/* Print new client data */
	std::cout << getTimestamp() << GREEN "New client connected successfully" CLEAR << std::endl;
	std::cout << "\t\t\t\taddress: " << inet_ntoa(clientAddress.sin_addr) <<  std::endl;
}

/* Read incoming data from client socket & perform actions */
void	Server::handleMessages(Client* client)
{	
	std::string	rawMessage;

	/* Client reads entire input string coming from their socket */
	client->read();

	/* Handle forcefully disconnected clients */
	if ((rawMessage = client->retrieveMessage()).empty() == true)
	{
		std::cout << getTimestamp() << RED "Removing disconnected client: " CLEAR << client->getUsername() << std::endl;
		removeClient(client);
	}
	else
	{
		/* While there are valid commands (messages) stored in the client's input string */
		while (rawMessage.empty() == false)
		{
			Message	msg(client, rawMessage);

			/* If command is nick but password has not been validated then remove user and break loop */
			if (msg.getCommand() == "nick" && !client->getPassStatus())
			{
				client->reply("ERROR :Closing Link: localhost (Bad Password)\n");
				removeClient(client);
				break ;
			}
			/* Attempt to execute command, ignoring CAP commands */
			else if (msg.getCommand() != "cap")
				executeCommand(msg);
			
			/* Retrieve next command */
			rawMessage = client->retrieveMessage();
		}
	}
}

/* Execute a command from client */
void	Server::executeCommand(const Message & msg)
{
	try {
		/* Attempt to execute command */
		_commands.at(msg.getCommand())->execute(msg);
	}
	/* Error message if command is invalid or not supported */
	catch(std::out_of_range &e) {
		std::cerr << RED "\t\t\t\tCommand '" << msg.getCommand() << "' was not found." CLEAR << std::endl;
	}
}

/* Main server loop */
void	Server::runServer(void)
{
	while (_status == ONLINE)
	{
		/* Poll all open sockets for activity */
		if (poll(_pfds.data(), _pfds.size(), 10) < 0)
			throw std::runtime_error("Error when attempting to poll");
			
		/* Iterate through sockets to check for events */
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			/* If there is an event is on the server socket, check for new connection */
			if (i == 0 && (_pfds[i].revents & POLLIN))
					handleConnections();
			else if (i > 0)
			{
				Client* client = _clients[i - 1];
				/* If there is an event on a client socket, get input */
				if (_pfds[i].revents & POLLIN)
					handleMessages(client);
				/* If there is no event, check if ping interval has passed and send PING to */
				else
				{
					if (client->getRegistration() && !client->getPingStatus() && 
						((std::time(nullptr) - client->getLastActivityTime()) > PING_INTERVAL))
					{
						client->reply(CMD_PING(_hostname, std::to_string(std::time(nullptr))));
						client->setPingStatus(true);
					}
				}
			}
		}
	}
}


/*******************************/
/*      Client Management      */
/*******************************/

/* Remove a client from the server */
void	Server::removeClient(Client* client)
{

	/* Remove client from all channels */
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
	{
		/* Remove ban from user to prevent stale memory pointer from remaining in _notMembers */
		it->second->setMemberModes(client, BAN, true);
		it->second->removeMember(client, CMD_PART(client->getNickname(), it->second->getName(), ""));
	}

	/* Remove client socket from pollFD vector */
	std::vector<pollfd>::iterator it2 = _pfds.begin();
	for (; it2 != _pfds.end(); ++it2)
	{
		if (it2->fd == client->getSocket())
		{
			_pfds.erase(it2);
			break;
		}
	}

	/* Shutdown socket & delete client */
	std::vector<Client *>::iterator it3 = find(_clients.begin(), _clients.end(), client);
	if (it3 != _clients.end())
	{
		shutdown(client->getSocket(), SHUT_RDWR);
		_clients.erase(it3);
	}
}

/* Check if specified nickname is already in use on server */
bool	Server::doesNickExist(const std::string nick) const {
	std::vector<Client *>::const_iterator	it = _clients.begin();
	std::vector<Client *>::const_iterator	ite = _clients.end();

	while (it != ite)
	{
		if ((*it)->getNickname() == nick)
			return (true);
		++it;
	}
	return (false);
}

/* Find and return client pointer using given nickname */
Client*		Server::getClientPtr(const std::string &client) {
	std::vector<Client*>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == client)
			break;
	}
	if (it == _clients.end())
	{
		std::cerr << RED "Client '" << client << "' was not found." CLEAR << std::endl;
		return (nullptr);
	}
	return *it;
}


/********************************/
/*      Channel Management      */
/********************************/

/* Create a new channel with given channel name */
void	Server::createChannel(const std::string& channel, Client* owner) {
	/* Check if channel already exists */
	if (!_channels.empty() && (_channels.find(channel) != _channels.end()))
		return;
	if (DEBUG)
		std::cout << GREEN "New channel created: " CLEAR << channel << std::endl << std::endl;
	_channels[channel] = new Channel(channel, owner);
}

/* Destroy channel with given channel name */
void	Server::destroyChannel(const std::string& channel) {
	/* Find correct channel in map */
	std::map<std::string, Channel *>::iterator it = _channels.find(channel);
	if (it == _channels.end())
		return;
	delete it->second;
	_channels.erase(it);
}

/* Check if a specified channel name already exists */
bool	Server::doesChannelExist(const std::string& channel) const {
	if (_channels.find(channel) != _channels.end())
		return (true);
	return (false);
}

/* Check if password provided for given channel is correct */
bool	Server::channelCheckPass(const std::string& channel, const std::string& pass) {
	std::map<std::string, Channel *>::iterator it = _channels.find(channel);
	/* Check if channel exists */
	if (it == _channels.end())
		return (false);
	/* Check if password match */
	if (it->second->getPass() == pass)
		return (true);
	return (false);
}

/* Find and return channel pointer using given channel name */
Channel*	Server::getChannelPtr(const std::string& channel) {
	std::map<std::string, Channel *>::iterator it = _channels.find(channel);
	if (it == _channels.end())
	{
		std::cerr << RED "Channel '" << channel << "' was not found." CLEAR << std::endl;
		return (nullptr);
	}
	return(it->second);
}

/* Return a formatted timestamp for current time */
const std::string	getTimestamp() {
	std::time_t time = std::time(nullptr);
	char output[50];
	int len;
	len = strftime(output, 50, "[%a %b %d %Y %X] : ", std::localtime(&time));
	output[len] = 0;
	return (std::string(output));
}

