#ifndef COMMAND_HPP
# define COMMAND_HPP

#pragma once

/* Systems Includes */
#include <string>

/* Local Includes */
#include "Message.hpp"
#include "Server.hpp"
#include "replies.hpp"
#include "Defines.h"

/* Class Prototypes */


class Command {
	public:
		/* Constructors & Destructor */
		
		virtual ~Command();

		/* Operator Overloads */

		/* Setters & Getters */

		/* Public Member Functions */
		virtual void	execute(const Message& msg) = 0;
		virtual bool	validate(const Message& msg) = 0;
		virtual bool	buildReply(const Message& msg) = 0;

	protected:
		Command(const std::string& name) : _name(name) { }
		const std::string	_name;
		bool				_channelOpRequired;
		bool				_globalOpRequired;
		int					_replCode;
		Server* 			_server;

		/* Member functions */
		std::string			_buildPrefix(const Message& msg) { 
			return msg._client->getNickname() + "!"
				 + msg._client->getUsername() + "@"
				 + _server->getHostname();
		}
};

#endif