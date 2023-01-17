#include "Server.hpp"
#include "defines.h"
/********************/
/* Command Includes */
/********************/

// #include "commands/Ban.hpp"
// #include "commands/Echo.hpp"
// #include "commands/Exit.hpp"
// #include "commands/Help.hpp"
// #include "commands/Info.hpp"
// #include "commands/Join.hpp"
// #include "commands/Kick.hpp"
// #include "commands/List.hpp"
// #include "commands/Mode.hpp"
// #include "commands/Names.hpp"
#include "commands/Nick.hpp"
// #include "commands/Ope.hpp"
// #include "commands/Part.hpp"
 #include "commands/Pass.hpp"
// #include "commands/Ping.hpp"
// #include "commands/Privmsg.hpp"
#include "commands/Quit.hpp"
#include "commands/User.hpp"

/*****************************/
/* Constructors & Destructor */
/*****************************/

Server::Server(const std::string& hostname, const int port, const std::string& password) :
	_hostname(hostname), _password(password), _port(port) {
	
	/* Setup server connection */
	initializeConnection();
	if (DEBUG)
	{
		std::cout << RED"Server initialization successful" CLEAR << std::endl;
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
		std::cout << RED"Server status: " CLEAR << GREEN"ONLINE" CLEAR << std::endl;
		std::cout << "_______________________________________" << std::endl << std::endl;
	}
	runServer();
}

Server::~Server() {
	/* Delete Commands*/
	std::map<std::string, Command *>::iterator it = _commands.begin();
	for (; it != _commands.end(); it++)
		delete it->second;

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

/*********************/
/* Setters & Getters */
/*********************/


/***************************/
/* Public Member Functions */
/***************************/

void	Server::initializeConnection(void) {
	int	yes = 1;

	/* Create socket */
	if ((_socket  = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw Server::socketException();

	/* Set socket as non-blocking */
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	/* Set socket options to reuse addresses */
	// FIXME: figure out why the SO_DEBUG doesn't work
	// if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | (DEBUG * SO_DEBUG), &yes, sizeof(int)) < 0)
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		// FIXME: temporary addition to see why SO_DEBUG fails setsockopt()
		// std::cerr << "setsockopt() failed " << _socket << "." << std::endl;
		// std::cerr << "errno set to " << errno << "." << std::endl;
		throw Server::socketException();
	}

	/* Setup socket address struct */
	_address.sin_port = htons(_port);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	//FIXME: convert to using hints and proper method for address struct

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

/* Intialize Commands Map */
void	Server::initializeCommands(void) {
	/* General Commands */
	// _commands["ping"] = new Ping();
	// _commands["info"] = new Info();
	// _commands["exit"] = new Exit();
	// _commands["echo"] = new Echo();
	// _commands["help"] = new Help();
	// _commands["ban"] = new Ban();
	// _commands["ope"] = new Ope();
	_commands["quit"] = new Quit(this);

	/* Channel Commands */
	// _commands["privmsg"] = new Privmsg();
	// _commands["join"] = new Join();
	// _commands["part"] = new Part();
	// _commands["list"] = new List();
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

/* Manage Connection Requests from New Clients */
void	Server::handleConnections()
{
	if (DEBUG)
		std::cout << RED"Incoming connection request" CLEAR<< std::endl;

	//FIXME: Pass addressinfo struct to get client data for implementing FTP later on
	int	new_fd;
	int	addressLen;
	struct sockaddr_in clientAddress;

	addressLen = sizeof(clientAddress);
	if ((new_fd = accept(_socket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen)) < 0)
		throw Server::acceptException();
	_clients.push_back(new Client(new_fd));
	_clients.back()->setAddress(clientAddress);
	pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);

	
	/* Print new client data */
	if (DEBUG)
	{
		std::cout << "New client connected successfully" << std::endl;
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
			std::cout << RED"Removing disconnected client: " CLEAR << client->getUsername() << std::endl;
		removeClient(client);
	}
	else
	{
		/* While there are valid commands (Messages) stored in the client's input string */
		while (rawMessage.empty() == false)
		{
			Message	msg(client, rawMessage);
			executeCommand(msg);
			rawMessage = client->retrieveMessage();
		}
	}
}

/* Execute a command from client */
void	Server::executeCommand(const Message & msg) {
	try {
		/* Check if command exists */
		_commands.at(msg.getCommand())->execute(msg);
	}
	catch(std::out_of_range &e) {
		std::cerr << "Command '" << msg.getCommand() 
				<< "' was not found." << std::endl;
	}
}

/* Main server loop */
void	Server::runServer(void) {
	while (_status == ONLINE) {

		if (poll(_pfds.data(), _pfds.size(), -1) < 0)
			throw Server::pollException();
			
		/* Iterate through sockets to check for events*/
		for (size_t i = 0; i < _pfds.size(); i++) {


			/* If any readable data is available */
			if (_pfds[i].revents & POLLIN) {

				/* If the event is on the server socket, check for new connection */
				if (_pfds[i].fd == _socket)
					handleConnections();

				/* If the event is on a client socket, deal with messages */
				else if (i > 0)
					handleMessages(_clients[i - 1]);
			}
		}
	}
}

/* Remove a client from the server */
void	Server::removeClient(Client* client) {
	/* Remove client from all channels */
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
		it->second->removeMember(client);

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

/* Check if a specified channel name already exists */
bool	Server::doesChannelExist(const std::string& channel) const {
	if (_channels.find(channel) != _channels.end())
		return (true);
	return (false);
}

/* Create a new channel with given channel name */
void	Server::createChannel(const std::string& channel, const std::string& pass, Client* owner) {
	/* Check if channel already exists */
	if (_channels.find(channel) == _channels.end())
		return;
	_channels["channel"] = new Channel(channel, pass, owner);
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
