#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#pragma once

/* System Includes */
#include <string>
#include <vector>

/* Local Includes */

/* Class Prototypes */
class Command;
class Client;

class Message {
	public:
		/* Constructors & Destructor */
		Message(Client* client, std::string raw);
		~Message();

		/* Setters & Getters */
		const 	std::string&				getCommand() const;
		const	std::string&				getPrefix() const;
		const	std::vector<std::string>&	getParams() const;

		// const	Client*						getClient() const;
	

		/*Utils*/
		const	std::string&				getReplies(int code) const;

		/* Public Attributes */
		Client*						_client;
	private:

		std::vector<std::string>	_params;
		std::string					_prefix;
		std::string					_cmd;	// Represents the command string, in lower case
		std::string					_store;	// Represents the remainder of a message, if it contained multipple valid commands separated by "\r\n"
};

#endif
