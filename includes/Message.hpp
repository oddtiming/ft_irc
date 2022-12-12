#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */

class User;

/* Class Prototypes */
class Command;

class Message {
	public:
		/* Constructors & Destructor */
		Message(User* user);
		~Message();

		/* Operator Overloads */

		/* Setters & Getters */

		/* Public Member Functions */

	private:
		Command *	_command;
		std::string	_raw;
		std::string _reply;
		User*		_owner;

};

#endif



