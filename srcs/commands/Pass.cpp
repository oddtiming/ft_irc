#include "commands/Pass.hpp"

Pass::Pass(Server* server) : Command("pass", server) {
	_channelOpRequired = true;
	_globalOpRequired = false;
}

Pass::~Pass() {

}

bool	Pass::validate(const Message& msg) {

	if (msg.getMiddle().empty())
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
		return false;
	}
	if (!msg._client->getUsername().empty())
	{
		msg._client->reply(ERR_ALREADYREGISTRED());
		std::cerr << "ERR_ALREADYREGISTRED" << std::endl;
		return false;
	}
	return true;
}

void	Pass::execute(const Message& msg) {

	if (validate(msg))
	{
		if (_server->getServerPassword() == msg.getMiddle().at(0))
			return ;
		else {
			msg._client->reply("ERROR :Closing Link: localhost (Bad Password)\n");
			_server->removeClient(msg._client);
		}

	}

}