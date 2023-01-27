#include "commands/Pass.hpp"

Pass::Pass(Server* server) : Command("pass", server), _password(server->getServerPassword()) { }

bool	Pass::validate(const Message& msg) {
	/* If no password is supplied, send error */
	if (msg.getMiddle().empty())
	{
		_client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
		return false;
	}
	/* If client has already registered send error */
	if (_client->getRegistration())
	{
		_client->reply(ERR_ALREADYREGISTRED(_server->getHostname()));
		return false;
	}
	return true;
}

void	Pass::execute(const Message& msg) {
	_client = msg._client;
	if (validate(msg))
	{
		/* If password matches set status */
		if (msg.getMiddle().at(0) == _password)
		{
			_client->setPassStatus(true);
			return ;
		}
		/* If password does not match send error and disconnect client */
		else 
		{
			_client->reply("ERROR :Closing Link: localhost (Bad Password)\n");
			_server->removeClient(_client);
		}
	}
}

//FIXME: Error message being sent twice