#include "commands/Quit.hpp"

Quit::Quit(Server* server) : Command("quit", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Quit::~Quit() {

}

bool	Quit::validate(const Message& msg) {

	std::string tmp = msg.getCommand();
	return (true);
}

void	Quit::execute(const Message& msg) {
	/* Check permissions for execution of function */
	//FIXME: check for proper response to quit
	std::string	message = "QUIT :";
	msg._client->reply(message);
	_server->removeClient(msg._client);
}