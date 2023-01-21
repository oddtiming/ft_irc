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
// #include "commands/Ope.hpp"
// #include "commands/Part.hpp"
 #include "commands/Pass.hpp"
#include "commands/Ping.hpp"
#include "commands/Pong.hpp"
#include "commands/Privmsg.hpp"
#include "commands/Quit.hpp"
#include "commands/User.hpp"
#include "Client.hpp"

/*****************************/
/* Constructor & Destructor */
/*****************************/

Server::Server(const std::string& hostname, const int port, const std::string& password) :
	_hostname(hostname), _password(password), _timeStart(std::time(nullptr)), _port(port) {
	
	/* Setup server connection */
	initializeConnection();
	if (DEBUG)
	{
		std::cout << RED "Server initialization successful" CLEAR << std::endl;
		std::cout << "	port: " << port << std::endl;
		std::cout << "	pass: " << password << std::endl;
	}
	
	/* Initialize commands map */
	initializeCommands();
	if (DEBUG)
	{
		std::cout << RED "Command initialization successful" CLEAR<< std::endl;
	}
	/* Start Server Loop */
	if (DEBUG)
	{
		std::cout << "_______________________________________" << std::endl << std::endl;
		std::cout << RED "Server status: " CLEAR << GREEN"ONLINE" CLEAR << std::endl;
		std::cout << "_______________________________________" << std::endl << std::endl;
	}
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
		throw Server::socketException();

	/* Set socket as non-blocking */
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	/* Set socket options to reuse addresses */
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw Server::socketException();

	/* Setup socket address struct */
	_address.sin_port = htons(_port);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	//FIXME: convert to using hints and proper method for address struct (confirm if this matters )

	/* Bind Socket */
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw Server::bindException();
	
	/* Set socket to passive listening */
	if (listen(_socket, MAX_CONNECTIONS) < 0)
		throw Server::listenException();
	
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
	// _commands["info"] = new Info();
	// _commands["exit"] = new Exit();
	// _commands["echo"] = new Echo();
	// _commands["help"] = new Help();
	// _commands["ban"] = new Ban();
	// _commands["ope"] = new Ope();
	_commands["quit"] = new Quit(this);
	_commands["mode"] = new Mode(this);
	_commands["away"] = new Away(this);

	/* Channel Commands */
	_commands["privmsg"] = new Privmsg(this);
	_commands["join"] = new Join(this);
	// _commands["part"] = new Part();
	 _commands["list"] = new List(this);
	// _commands["names"] = new Names();
	// _commands["kick"] = new Kick();
	_commands["nick"] = new Nick(this);
	_commands["user"] = new User(this);
	_commands["pass"] = new Pass(this);
	// _commands["mode"] = new Mode();
	//_commands["who"] = new Who();
	//_commands["time"] = new Time();
	//_commands["invite"] = new Invite();
	//_commands["motd"] = new Motd();
}


/****************************************/
/*      Server Operation Functions      */
/****************************************/

/* Manage Connection Requests from New Clients */
void	Server::handleConnections()
{
	//FIXME: Clean up server status printouts, add timestamps, and add logging to file
	if (DEBUG)
		std::cout << RED"Incoming connection request" CLEAR<< std::endl;

	int	new_fd;
	int	addressLen;
	struct sockaddr_in clientAddress;
	addressLen = sizeof(clientAddress);

	/* Attempt to connect to client and get client address info*/
	if ((new_fd = accept(_socket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen)) < 0)
		throw Server::acceptException();

	/* Add client to _clients map and populate address variables */
	_clients.push_back(new Client(new_fd));
	_clients.back()->setAddress(clientAddress);
	_clients.back()->setHostname(inet_ntoa(clientAddress.sin_addr));

	/* Create new pollfd object and add to _pfds list */
	pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);

	
	/* Print new client data */
	if (DEBUG)
	{
		std::cout << GREEN "New client connected successfully" CLEAR << std::endl;
		std::cout << "	address: " << inet_ntoa(clientAddress.sin_addr) <<  std::endl << std::endl;
	}

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
		if (DEBUG)
			std::cout << RED "Removing disconnected client: " CLEAR << client->getUsername() << std::endl;
		removeClient(client);
	}
	else
	{
		/* While there are valid commands (messages) stored in the client's input string */
		while (rawMessage.empty() == false)
		{
			Message	msg(client, rawMessage);

			/* Handle default response to capabilities requests */
			if (msg.getCommand() == "CAP")
				msg._client->reply("CAP * LS :");
			/* Attempt to execute command */
			else
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
		std::cerr << "Command '" << msg.getCommand() 
				<< "' was not found." << std::endl;
	}
}

/* Main server loop */
void	Server::runServer(void)
{
	while (_status == ONLINE)
	{
		/* Poll all open sockets for activity */
		if (poll(_pfds.data(), _pfds.size(), 10) < 0)
			throw Server::pollException();
			
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
				//FIXME: Do we want to auto disconnect clients after a certain period of inactivity?
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
		it->second->removeMember(client);
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



//NOTE: Formatting for timestamps
/*
std::time_t result = std::time(nullptr);
	char Output[19];
	strftime(Output, 19, "[%Y%m%d_%H%M%S] ", std::localtime(&result));
	std::cout << Output;

*/