#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#pragma once

/* System Includes */
#include <string>
#include <vector>

/* Local Includes */

class User;

/* Class Prototypes */
class Command;

class Message {
	public:
		/* Constructors & Destructor */
		Message(User* user, std::string raw);
		~Message();

		/* Setters & Getters */
		const 	std::string&				getCommand() const;
		const	std::string&				getPrefix() const;
		const	std::vector<std::string>&	getParams() const;

		/*Utils*/
		const	std::string&				getReplies(int code) const;

	private:
		const	std::vector<std::string>	_params;
		const	std::string					_prefix;
		const	std::string					_cmd;	// Represents the command string, in lower case
		const	User*						_user;
};

#endif