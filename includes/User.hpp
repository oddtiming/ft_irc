#ifndef USER_HPP
# define USER_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>

/* Local Includes */
#include "Channel.hpp"

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
		bool				checkChannelModes(const char& mode);

		void				setGlobalModes(const char& mode);
		bool				checkGlobalModes(const char& mode);

		/* Public Member Functions */

	private:
		const int					_userSocket;
		std::string					_nickname;
		std::string					_username;
		std::string					_password;
		std::map<Channel *, char>	_channelModes;		/* Per channel member modes for each user */
		char						_globalModes;		/* Mode flags stored using bitmask */
};


#endif