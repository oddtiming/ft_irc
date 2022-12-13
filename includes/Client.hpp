#ifndef CLIENT_HPP
# define CLIENT_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>
#include <sys/socket.h>

/* Local Includes */
#include "Channel.hpp"
#include "Message.hpp"

class Client {
	public:
		/* Constructors & Destructor */
		Client(int sockFD);
		~Client();

		/* Operator Overloads */

		/* Setters & Getters */
		void				setNickname(const std::string& nickname);
		const std::string&	getNickname(void);

		void				setClientname(const std::string& nickname);
		const std::string&	getClientname(void);

		void				setPassword(const std::string& nickname);
		const std::string&	getPassword(void);

		
		void				setChannelModes(const char& mode);
		bool				checkChannelModes(const std::string& channel, const char& mode);

		void				setGlobalModes(const char& mode);
		bool				checkGlobalModes(const char& mode);

		/* Public Member Functions */
		Message	read();
		void	reply(const std::string& msg);

	private:
		const int						_socket;
		std::string						_nickname;
		std::string						_Clientname;
		std::string						_password;
		char							_globalModes;		/* Mode flags stored using bitmask */
		std::map<std::string, char>		_channelModes;		/* Per client channel modes */
};

#endif