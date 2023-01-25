#include "commands/Part.hpp"

/* Constructors & Destructor */
Part::Part(Server* server) : Command("part", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Part::~Part() {
}

bool	Part::parse(const Message& msg)
{
    if (msg.getMiddle().empty())
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return false;
	}

	std::string raw = msg.getMiddle().at(0);
	size_t pos;

	while ((pos = raw.find(',')) != std::string::npos)
	{
		_targets.push_back(raw.substr(0, pos));
		raw.erase(0, pos + 1);
	}
    if (raw.size() > 0)
		_targets.push_back(raw);
	
    _partMsg = msg.getTrailing();
	
	return true;
}

/* Public Member Functions */
bool	Part::validate(const std::string& channel, Client *client)
{	
	Channel * target = _server->getChannelPtr(channel);

	if (target == nullptr)
	{
		client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), client->getNickname(), channel));
		return false;
	}
	
	if (target->isMember(client) == false)
	{
		client->reply(ERR_NOTONCHANNEL(channel));
		return false;
	}
	
	_currTarget = target;

	return true;
}


void	Part::execute(const Message& msg) {

	if (parse(msg) == false)
		return ;

	std::vector<std::string>::iterator	ite = _targets.end();

	for (std::vector<std::string>::iterator	it = _targets.begin(); it != ite; ++it)
	{
		if (validate(*it, msg._client))
			_currTarget->removeMember(msg._client, CMD_PART(_buildPrefix(msg), *it, _partMsg));
	}

}

std::string Part::_validChanPrefixes = "#&!+";