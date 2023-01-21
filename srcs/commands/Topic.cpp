#include "../../includes/commands/Topic.hpp"

Topic::Topic(Server* server) : Command("topic", server) {

}

Topic::~Topic() {

}

bool	Topic::validate(const Message& msg) {
	if (msg.getMiddle().empty()) {
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return false;
	}
	_target = msg.getMiddle().at(0);
	/*if (!_server->getChannelPtr(_target)->checkMemberModes(msg._client, 'o')) {
		msg._client->reply(ERR_CHANOPRIVSNEEDED(_target));
		return false;
	}*/
	
	Channel *target = _server->getChannelPtr(_target);

	if (target == nullptr)
	{
		msg._client->reply(ERR_NOSUCHCHANNEL(_target));
		return false;
	}

	if (target->isMember(msg._client) == false)
	{
		msg._client->reply(ERR_NOTONCHANNEL(_target));
		return false;
	}
	return true;
}

void	Topic::execute(const Message& msg) {
	
	if (validate(msg))
	{
		if (msg.getTrailing().empty())
			msg._client->reply(RPL_TOPIC(_target, _server->getChannelPtr(_target)->getTopic()));
		if (msg.getTrailing().size() == 1 && msg.getTrailing().at(0) == ':')
			_server->getChannelPtr(_target)->setTopic(" ");
		else
			_server->getChannelPtr(_target)->setTopic(msg.getTrailing());
	}
}
