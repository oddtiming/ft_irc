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
		_client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return false;
	}
	if (!_client->getUsername().empty())
	{
		_client->reply(ERR_ALREADYREGISTRED());
		return false;
	}
	return true;
}

void	Pass::execute(const Message& msg) {
	_client = msg._client;

	if (validate(msg))
	{
		if (_server->getServerPassword() == msg.getMiddle().at(0))
		{
			_client->setPassStatus(true);
			return ;
		}
		else 
		{
			_client->reply("ERROR :Closing Link: localhost (Bad Password)\n");
			_server->removeClient(_client);
		}
	}
}