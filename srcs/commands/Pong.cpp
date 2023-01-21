#include "commands/Pong.hpp"

/* Constructors & Destructor */
Pong::Pong(Server *server) : Command("pong", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Pong::~Pong() {
}

/* Public Member Functions */
void	Pong::execute(const Message& msg) {
	/* When PONG is received, reset PING status for client */
	msg._client->setPingStatus(false);
}
