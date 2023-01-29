#include "commands/Pong.hpp"

Pong::Pong(Server *server) : Command("pong", server) { }

void	Pong::execute(const Message& msg) {
	/* When PONG is received, reset PING status for client */
	msg._client->setPingStatus(false);
}
