#ifndef USER_HPP
# define USER_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>
#include <sys/socket.h>

/* Local Includes */
#include "Channel.hpp"
#include "Message.hpp"

class User {
	public:
		/* Constructors & Destructor */
		User(int sockFD);
		~User();

		/* Operator Overloads */

		/* Setters & Getters */
		void				setNickname(const std::string& nickname);
		const std::string&	getNickname(void);

		void				setUsername(const std::string& nickname);
		const std::string&	getUsername(void);

		void				setPassword(const std::string& nickname);
		const std::string&	getPassword(void);

		
		void				setChannelModes(const char& mode);
		bool				checkChannelModes(const std::string& channel, const char& mode);

		void				setGlobalModes(const char& mode);
		bool				checkGlobalModes(const char& mode);

		/* Public Member Functions */
		Message*	read();

	private:
		const int						_socket;
		std::string						_nickname;
		std::string						_username;
		std::string						_password;
		char							_globalModes;		/* Mode flags stored using bitmask */
		std::map<std::string, char>		_channelModes;		/* Per client channel modes */
};


#endif