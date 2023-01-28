#ifndef COMMAND_HPP
#define COMMAND_HPP

#pragma once

/* Systems Includes */
#include <string>

/* Local Includes */
#include "Message.hpp"
#include "Server.hpp"
#include "defines.h"
#include "replies.h"

/* Class Prototypes */

class Command
{
  public:
	/* Constructors & Destructor */

	virtual ~Command( ) {}

	/* Operator Overloads */

	/* Setters & Getters */

	/* Public Member Functions */
	virtual void execute(const Message& msg) = 0;

  protected:
	Command(const std::string& name, Server* server) : _name(name), _server(server) {}
	const std::string _name;
	bool              _channelOpRequired;
	bool              _globalOpRequired;
	int               _replCode;
	Server*           _server;

	/* Member functions */
	std::string _buildPrefix(const Message& msg) {
		return msg._client->getNickname( ) + "!" + msg._client->getUsername( ) + "@"
		       + msg._client->getHostname( );
		// FIXME: DO NOT FORGET TO CHANGE THIS, hardcoded for testing
	}
};

#endif