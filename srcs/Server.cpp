#include "Server.hpp"

/* Command Includes */
#include "commands/Ban.hpp"
#include "commands/Echo.hpp"
#include "commands/Exit.hpp"
#include "commands/Help.hpp"
#include "commands/Info.hpp"
#include "commands/Join.hpp"
#include "commands/Kick.hpp"
#include "commands/List.hpp"
#include "commands/Mode.hpp"
#include "commands/Names.hpp"
#include "commands/Nick.hpp"
#include "commands/Ope.hpp"
#include "commands/Part.hpp"
#include "commands/Pass.hpp"
#include "commands/Ping.hpp"
#include "commands/Privmsg.hpp"
#include "commands/Quit.hpp"
#include "commands/User.hpp"

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

/* Setters & Getters */


/* Public Member Functions */

void	Server::initializeServer(void) {
	int	yes = 1;

	/* Create socket */
	if ((_socket  = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw Server::socketException();

	/* Set socket as non-blocking */
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	/* Set socket options to reuse addresses */
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | (DEBUG * SO_DEBUG), &yes, sizeof(int)) < 0)
		throw Server::socketException();

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
	_commands["ping"] = new Ping();
	_commands["info"] = new Info();
	_commands["exit"] = new Exit();
	_commands["echo"] = new Echo();
	_commands["help"] = new Help();
	_commands["ban"] = new Ban();
	_commands["ope"] = new Ope();
	_commands["quit"] = new Quit();

	/* Channel Commands */
	_commands["privmsg"] = new Privmsg();
	_commands["join"] = new Join();
	_commands["part"] = new Part();
	_commands["list"] = new List();
	_commands["names"] = new Names();
	_commands["kick"] = new Kick();
	_commands["nick"] = new Nick();
	_commands["user"] = new User();
	_commands["pass"] = new Pass();
	_commands["mode"] = new Mode();

	//_commands["who"] = new Who();
	//_commands["time"] = new Time();
	//_commands["invite"] = new Invite();
	//_commands["motd"] = new Motd();
	

}

/* Manage Connection Requests from New Clients */
void	Server::handleConnections()
{
	//FIXME: Pass addressinfo struct to get client data for implementing FTP later on
	int	new_fd;
	if ((new_fd = accept(_socket, NULL, NULL)) < 0)
		throw Server::acceptException();
	_clients.push_back(new Client(new_fd));
	pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);
	std::cout << "New client has connected to server" << std::endl;
}

/* Read incoming data from client socket & perform actions */
void	Server::handleMessages(Client* client)
{
	Message	msg = client->read();

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
					handleMessages(_clients[i - 1]);
			}
		}
	}
}

/* This weird custom functor is to allow embedded find_if() */
// Reference: https://stackoverflow.com/questions/14437825
struct nick_equal : std::unary_function<Client *, std::string>
{
    nick_equal(const std::string& nick) : _nick(nick) {}
    bool operator()(const Client * client) const { return client->getNickname() == _nick; }

    const std::string&	_nick;
};


bool	Server::doesNickExist(const std::string nick) const {
	nick_equal init(nick);

	if(find_if(_clients.begin(), _clients.end(), nick_equal(nick)) == _clients.end())
		return (false);
	return (true);

	/* As opposed to */
	// std::vector<Client *>::const_iterator	it = _clients.begin();
	// std::vector<Client *>::const_iterator	ite = _clients.end();

	// while (it != ite)
	// {
	// 	if (*(it)->getNickname() == nick)
	// 		return (true);
	// 	++it;
	// }
	// return (false);
}
