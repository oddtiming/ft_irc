#include "commands/Nick.hpp"
#include "Server.hpp"

Nick::Nick(Server* server) : Command("nick", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();

	/* Check if a nickname was given */
	if (middle.empty())
	{
		_client->reply(ERR_NONICKNAMEGIVEN(_server->getHostname()));
		return (false);
	}
	_nick = middle.at(0);
	/* If nickname is too long, return error */
	if (_nick.size() > 9)
	{
		_client->reply(ERR_ERRONEUSNICKNAME(_server->getHostname(), _nick));
		return false;
	}
	/* If provided nickname is the same as current nickname, do nothing */
	if (_client->getNickname() == _nick)
		return (false);
	/* If nickname is already in use, return error */
	if (_server->doesNickExist(_nick))
	{
		_client->reply(ERR_NICKNAMEINUSE(_server->getHostname(), _nick));
		return (false);
	}
	return true;
}

void Nick::execute(const Message &msg) {
	_client = msg._client;

	/* Attempt to validate nickname */
	if (validate(msg)) {
		if (_server->doesNickExist(_nick))
			_client->reply(ERR_NICKNAMEINUSE(_server->getHostname(), _nick));
		else
		{
			/* If client already registered, send notification of nickname change */
			if (_client->getRegistration())
				_client->reply(CMD_NICK(_buildPrefix(msg), _nick));
			_client->setNickname(_nick);
		}
	}
	 /* If username and nickname have been successfully added, register user*/
    if (!_client->getUsername().empty() && !_client->getNickname().empty() && !_client->getRegistration())
    {
        _client->setRegistration(true);
        _client->reply(RPL_WELCOME(_server->getHostname(), _client->getNickname(), _buildPrefix(msg)));
		if (DEBUG)
        	std::cout << getTimestamp() << GREEN "New user successfully registered: " CLEAR << _client->getNickname() << std::endl << std::endl;
    }

}