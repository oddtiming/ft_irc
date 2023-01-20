#include "commands/Ping.hpp"

/* Constructors & Destructor */
Ping::Ping(Server *server) : Command("ping", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Ping::~Ping() {
}

/* Public Member Functions */
void	Ping::execute(const Message& msg) {
	std::string token;
	/* If anything other than just PING received sent back same token, otherwise send empty string */
	if (!msg.getMiddle().empty())
		token = msg.getMiddle().at(0);
	msg._client->reply(CMD_PONG(_server->getHostname(), token));
}