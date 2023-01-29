#include "commands/Part.hpp"

Part::Part(Server* server) : Command("part", server) { }

/* Parse msg to get list of all channels */
bool	Part::parse(const Message& msg)
{
	/* If message is empty send error reply */
    if (msg.getMiddle().empty())
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
		return false;
	}

	std::string raw = msg.getMiddle().at(0);
	size_t pos;

	/* Iterate through string and split out all channel names */
	while ((pos = raw.find(',')) != std::string::npos)
	{
		_targetChannels.push_back(raw.substr(0, pos));
		raw.erase(0, pos + 1);
	}
    if (raw.size() > 0)
		_targetChannels.push_back(raw);
	
	/* Get part message */
    _message = msg.getTrailing();
	
	return true;
}

/* Run validation to ensure command can be executed */
bool	Part::validate(const std::string& channel, Client *client) {	
	Channel* target = _server->getChannelPtr(channel);

	if (target == nullptr)
	{
		client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), client->getNickname(), channel));
		return false;
	}
	
	if (target->isMember(client) == false)
	{
		client->reply(ERR_NOTONCHANNEL(_server->getHostname(), _client->getNickname(), channel));
		return false;
	}
	
	_currTarget = target;

	return true;
}

/* Control function for command */
void	Part::execute(const Message& msg) {
	clearData();
	_client = msg._client;

	/* Attempt to parse command parameters */
	if (parse(msg) == false)
		return ;

	/* Iterate through target channels and attempt to validate and execute command for each one */
	std::vector<std::string>::iterator	ite = _targetChannels.end();

	for (std::vector<std::string>::iterator	it = _targetChannels.begin(); it != ite; ++it)
	{
		if (validate(*it, msg._client))
		{
			/* Send reply to all channel members */
			_currTarget->sendToAll(CMD_PART(_buildPrefix(msg), *it, _message));

			/* Remove members from channel */
			_currTarget->removeMember(_client);

			/* If channel is empty, delete it */
			if (_currTarget->getIsEmpty())
				_server->destroyChannel(_currTarget->getName());
		}
	}

}

/* Clear all data from previous function calls */
void	Part::clearData() {
	_client = nullptr;
	_currTarget = nullptr;
	_targetChannels.clear();
	_message.clear();
}