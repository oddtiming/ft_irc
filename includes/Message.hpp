#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#pragma once

/* System Includes */
#include <string>
#include <vector>
#include <algorithm>

/* Class Prototypes */
class Command;
class Client;

class Message {
	public:
		/* Constructor & Destructor */
		Message(Client* client, std::string raw);
		~Message() { }

		/* Setters & Getters */
		const	std::string&				getRaw() const			{ return _raw; }
		const 	std::string&				getCommand() const		{ return _cmd; }
		const	std::string&				getPrefix() const		{ return _prefix; }
		const	std::string&				getTrailing() const 	{ return _trailing; }
		const	std::vector<std::string>&	getMiddle() const		{ return _middle; }
		const bool&							hasPrefix() const		{ return _hasPrefix; }
		const bool&							hasMiddle() const		{ return _hasMiddle; }
		const bool&							hasTrailing() const 	{ return _hasTrailing; }


		/* Public Attributes */
		Client*						_client;

	private:
		std::string 				_raw;			/* Raw input from recv() call */
		std::string					_cmd;
		std::string					_prefix;		/* Parameters before initial ':' (unlikely to ever occur) */
		bool						_hasPrefix;
		std::vector<std::string>	_middle;		/* Additional parameters for command */
		bool						_hasMiddle;
		std::string					_trailing;		/* Parameters after second ':' (any other colons after this are part of message string) */
		bool						_hasTrailing;
};

#endif