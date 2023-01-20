#include "commands/Ping.hpp"

/* Constructors & Destructor */
Ping::Ping(Server *server) : Command("ping", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Ping::~Ping() {
}

/* Public Member Functions */
bool	Ping::validate(const Message& msg) {

}


void	Ping::execute(const Message& msg) {

	/* Check permissions for execution of function */
	msg._client->reply(CMD_PING(_server->getHostname()));
}


/*
PONG REPLY
:serverhost PONG serverhost :serverhost
*/

// :penguin.omega.example.org PONG penguin.omega.example.org :penguin.omega.example.org