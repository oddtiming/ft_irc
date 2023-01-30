#ifndef SERVER_HPP
# define SERVER_HPP

#pragma once

/* System Includes */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> //MSG_NOSIGNAL
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
#include <ctime>
#include <unistd.h>
#include <netdb.h>
#include <exception>

/* Local Includes */
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "defines.h"

/* Class Prototypes */
class Command;

class Server {
	public:
		Server(const std::string& servername, const int port, const std::string& password);
		~Server();
		
		/* Setters & Getters */
		const std::string&					getServerPassword(void) const	{ return _password; }
		const std::string&					getHostname(void) const 		{ return _hostname; }
		const std::time_t&					getStartTime(void) const 		{ return _timeStart; }
		const std::string&					getServername(void) const 		{ return _servername; }
	
		/*************************/
		/*    Server Operation   */
		/*************************/
		void								initializeConnection();
		void								initializeCommands();
		void								runServer();
		void								stopServer();
		void								stopServer(int signum);	/* Overload for signal() */
		void								handleConnections();
		void								handleMessages(Client* client);
		void								executeCommand(const Message & msg);
		
		/*************************/
		/*   Client Management   */
		/*************************/
		bool								doesNickExist(const std::string nick) const;
		Client* 							getClientPtr(const std::string& client);
		Channel*							getChannelPtr(const std::string& channel);
		void								removeClient(Client* client);

		/************************/
		/*  Channel Management  */
		/************************/
		bool								doesChannelExist(const std::string& channel) const;
		void								createChannel(const std::string& channel, Client* owner);
		void								destroyChannel(const std::string& channel);
		bool								channelCheckPass(const std::string& channel, const std::string& pass);
		std::map<std::string, Channel *>	getChannelList() { return _channels; }

	private:
		/* General Server Data */
		std::string							_servername;
		std::string							_hostname;
		std::string							_password;
		const std::time_t					_timeStart;

		/* Networking Data */
		const int							_port;
		struct sockaddr_in					_address;
		int									_socket;
		std::vector<pollfd>					_pfds;
		std::string							_ip;

		/* IRC Server Data */
		std::vector<Client *>				_clients;
		std::map<std::string, Channel *>	_channels;
		std::map<std::string, Command *>	_commands;
};

/* Non-member functions */
const std::string	getTimestamp();

#endif
