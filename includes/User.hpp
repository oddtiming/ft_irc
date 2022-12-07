#ifndef USER_HPP
# define USER_HPP

#pragma once

#include <string>

class User {
	public:
		/* Constructors & Destructor */
		User(int sockFD);
		~User();

		/* Operator Overloads */

		/* Setters & Getters */
		const std::string&	getNickname(void);
		const std::string&	getUsername(void);
		const std::string&	getPassword(void);

		void	setModes(const char& mode);

		
		/* Public Member Functions */
		bool	checkModes(const char& mode);

	private:
		int			_userSocket;

		std::string	_nickname;
		std::string	_username;
		std::string	_password;

		char16_t	_modes;		/* Mode flags stored using bitmask */
};


#endif