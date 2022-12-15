#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#pragma once

/* System Includes */
#include <string>
#include <vector>
#include <iostream>		// FIXME: for std::cerr DEBUG, to remove
#include <algorithm>

/* Local Includes */

/* Class Prototypes */
class Command;
class Client;

class Message {
	public:
		/* Constructors & Destructor */
		Message(Client* client, std::string raw);
		~Message() { }

		/* Setters & Getters */
		const 	std::string&				getCommand() const { return _cmd; }
		const	std::string&				getPrefix() const { return _prefix; }
		const	std::string&				getTrailing() const { return _trailing; }
		const	std::vector<std::string>&	getMiddle() const { return _middle; }

		/* Public Attributes */
		Client*						_client;

	private:
		std::vector<std::string>	_middle;
		std::string					_prefix;
		std::string					_trailing;
		std::string					_cmd;	// Represents the command string, in lower case
};

#endif
