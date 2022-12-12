#include "Server.hpp"
#include "commands/Join.hpp"

/* Constructors & Destructor */
Server::Server(const std::string& hostname, const int port, const std::string& password) :
	_hostname(hostname), _password(password), _port(port) {
	
	/* Setup server connection */
	initializeServer();
	
	/* Initialize commands map */
	initializeCommands();

	/* Start Server Loop */
	runServer();
}

Server::~Server() {
	/* Delete Commands*/
	std::map<std::string, Command *>::iterator it = _commands.begin();
	for (; it != _commands.end(); it++)
		delete it->second;

	/* Delete Users */
	for (size_t i = 0; i < _users.size(); i++)
		delete (_users[i]);
	_users.clear();
	//Ensure socket it closed in destructor for each user
	
	/* Delete channels */
	std::map<std::string, Channel *>::iterator it_ch = _channels.begin();
	for (; it_ch != _channels.end(); it_ch++)
		delete (it_ch->second);
	_channels.clear();

	/* Close server socket */
	shutdown(_socket, SHUT_RDWR);
}

/* Public Member Functions */

void	Server::initializeServer(void) {
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

	std::cout << "port: " << _port << std::endl;
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
	// _commands["ping"] = 
	// _commands["info"] = 
	// _commands["exit"] = 
	// _commands["echo"] = 
	// _commands["help"] = 
	// _commands["ban"] =
	// _commands["ope"] =
	// _commands["quit"] =

	// // Channel commands
	// _commands["privmsg"] =
	_commands["join"] = new Join();
	// _commands["part"] =
	// _commands["list"] = 
	// _commands["names"] =
	// _commands["kick"] = new Kick();
	// _commands["nick"] = new Nick();
	// _commands["user"] = new User();
	// _commands["pass"] = new Pass();
	// _commands["mode"] = new Mode();
}

/* Manage Connection Requests from New Clients */
void	Server::handleConnections()
{
	int	new_fd;
	if ((new_fd = accept(_socket, NULL, NULL)) < 0)
		throw Server::acceptException();
	_users.push_back(new User(new_fd));
	pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);
	std::cout << "New client has connected to server" << std::endl;
}

/* Read incoming data from client socket & perform actions */
void	Server::handleMessages(User* user)
{
	Message	msg = user->read();

	try{
		_commands.at(msg.getCommand())->execute(msg);
	}
	catch(std::out_of_range &e) {
		std::cerr << "Command " << msg.getCommand() 
				  << " was not found." << std::endl;
	}
}

void	Server::runServer(void) {
	/* Run main server code in this loop */
	while (_status == ONLINE) {

		/* Poll open sockets */
		if (poll(_pfds.data(), _pfds.size(), -1) < 0)
			throw Server::pollException();
			
		/* Iterate through sockets and check for events*/
		for (size_t i = 0; i < _pfds.size(); i++) {
			/* If any readable data is available */
			if (_pfds[i].revents & POLLIN) {
				/* If the event is on the server socket, check for new connection */
				if (_pfds[i].fd == _socket)
					handleConnections();
				/* If the event is on a client socket, deal with messages */
				else if (i > 0)
					handleMessages(_users[i - 1]);
			}
		}
	}
}
