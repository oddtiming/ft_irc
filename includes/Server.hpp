#ifndef SERVER_HPP
# define SERVER_HPP

#pragma once

/* System Includes */
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <string>
#include <iostream>		// cout, cerr, endl
#include <algorithm>	// find_if
#include <iomanip>
#include <vector>
#include <map>

/* Local Includes */
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"

/* Class Prototypes */
class Command;

#define MAX_CONNECTIONS 10

typedef enum s_serverStatus {
	OFFLINE = 0,
	ONLINE = 1,
	CLOSED = 2
}	t_serverStatus;

class Server {
	public:
		Server(const std::string& hostname, const int port, const std::string& password);
		~Server();
		

		/* Setters & Getters */
		const std::string& getServerPassword(void) const { return _password; }
		const std::string& getHostname(void) const { return _hostname; }

	
		/*************************/
		/*    Server Operation   */
		/*************************/
		void	initializeConnection();
		void	initializeCommands();
		void	runServer();
		void	handleConnections();
		void	handleMessages(Client* client);
		void	executeCommand(const Message & msg);
		
		/*************************/
		/*   Client Management   */
		/*************************/
		bool		doesUserExist(const std::string user) const;
		bool		doesNickExist(const std::string nick) const;
		Client* 	getClientPtr(const std::string& client);
		Channel*	getChannelPtr(const std::string& channel);
		// FIXME: not sure if Client * needs to be const. Needs to be implemented anyways

		void	removeClient(Client* client);


		/************************/
		/*  Channel Management  */
		/************************/
		bool	doesChannelExist(const std::string& channel) const;
		bool	isUserChannelMember(const std::string& user) const;
		void	createChannel(const std::string& channel, Client* owner);
		void	destroyChannel(const std::string& channel);
		bool	channelCheckPass(const std::string& channel, const std::string& pass);


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
