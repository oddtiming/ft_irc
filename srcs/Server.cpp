#include "Server.hpp"


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
	/* Free serverAddress struct */

	/* Shutdown open connections */

	/* Close open FDs */
}

/* Operator Overloads */

/* Setters & Getters */

/* Public Member Functions */

void	Server::initializeServer(void) {
	/* Create socket */
	if ((_socket  = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw Server::socketException();

	/* Set socket as non-blocking */
	fcntl(_socket, F_SETFL, O_NONBLOCK);

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
}

void	Server::runServer(void) {
	
	/* For testing only */
	char	serv_msg[1024] = "Connection to server successful";


	pollfd pfd = {.fd = _socket, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);

	/* Run main server code in this loop */
	while (this->_status == ONLINE) {

		/* Check for error returns from pollfds */
		if (poll(_pfds.data(), _pfds.size(), -1) == -1)
			throw	Server::pollException();
		/* Iterate through pollfds and check for events*/
		for (size_t i = 0; i < _pfds.size(); i++) {
			/* If an event is detected */
			if (_pfds[i].revents & POLLIN) {
				/* If the event is on the server socket, check for new connection */
				if (_pfds[i].fd == _socket) {
					int	new_fd;
					if ((new_fd = accept(_socket, NULL, NULL)) < 0)
						throw Server::acceptException();
					_users.push_back(new User(new_fd));
					pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
					_pfds.push_back(pfd);

					std::cout << "New client has connected to server" << std::endl;
					
					/* For testing only */
					send(new_fd, serv_msg, sizeof(serv_msg), 0);
				}
				/* If the event is on a client socket */
				else if (i > 0) {
					/* Handle all messaging and commands*/
					Message* message = _users[i - 1]->read();

				}
			}
		}
	}
}
