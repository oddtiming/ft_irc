#ifndef SERVER_HPP
# define SERVER_HPP

#pragma once

/* System Includes */
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

/* Local Includes */
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"

/* Class Prototypes */
class Command;

#define MAX_CONNECTIONS 5

// FIXME: set back to 0 before corrections, makes use of the recvmsg() function
#define DEBUG 1

typedef enum s_serverStatus {
	OFFLINE = 0,
	ONLINE = 1,
	CLOSED = 2
}	t_serverStatus;

class Server {
	public:
		/* Constructors & Destructors */
		Server();
		Server(const std::string& hostname, const int port, const std::string& password);
		~Server();
		
		/* Operator Overloads */

		/* Setters & Getters */
		const std::string& getServerPassword(void);

		/* Public Member Functions */
		void	initializeServer();
		void	initializeCommands();
		void	runServer();

		void	handleConnections();
		void	handleMessages(Client* client);


		/* Exceptions */
		class socketException : public std::exception {
			public:
				const char*	what() const throw() {
					return("Unable to create socket");
				}
		};

		class bindException : public std::exception {
			public:
				const char*	what() const throw() {
					return("Unable to bind socket to port");
				}
		};

		class listenException : public std::exception {
			public:
				const char*	what() const throw() {
					return("Unable to set socket to passive");
				}
		};
		class pollException : public std::exception {
			public:
				const char*	what() const throw() {
					return("Error when polling");
				}
		};
		class acceptException : public std::exception {
			public:
				const char*	what() const throw() {
					return("Error when accepting");
				}
		};


	private:
		/* General Server Data */
		std::string							_hostname;
		std::string							_password;
		int									_status;

		/* Networking Data */
		const int							_port;
		struct sockaddr_in					_address;
		int									_socket;
		std::vector<pollfd>					_pfds;

		/* IRC Server Data */
		std::vector<Client *>				_clients;
		std::map<std::string, Channel *>	_channels;
		std::map<std::string, Command *>	_commands;
};

#endif