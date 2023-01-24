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
	if (!_server->doesChannelExist(_target)) {
		msg._client->reply(ERR_NOSUCHCHANNEL(_target));
		return false;
	}
	if (_target.empty())
		return false;
	if ((msg.hasTrailing() || msg.getMiddle().size() > 1) && _server->getChannelPtr(_target)->checkModes(TOPIC_SET_OP) && !_server->getChannelPtr(_target)->checkMemberModes(msg._client, C_OP)) {
		msg._client->reply(ERR_CHANOPRIVSNEEDED(_target));
		return false;
	}
	if (!_server->getChannelPtr(_target)->isMember(msg._client)) {
		msg._client->reply(ERR_NOTONCHANNEL(_target));
		return false;
	}
	return true;
}
//fixme: add a reply all to topic
void	Topic::execute(const Message& msg) {
	if (validate(msg))
	{
		if (msg.getTrailing().size() == 1 && msg.getTrailing().at(0) == ':') {
			_server->getChannelPtr(_target)->setTopic("");
			_server->getChannelPtr(_target)->sendToAll(":" + _buildPrefix(msg) + " TOPIC " + _target + " : " + "\r\n");
		}
		else if (msg.hasTrailing()) {
			_server->getChannelPtr(_target)->setTopic(msg.getTrailing());
			_server->getChannelPtr(_target)->sendToAll(":" + _buildPrefix(msg) + " TOPIC " + _target + " :"+ msg.getTrailing() + "\r\n");
	}
		else if (!msg.hasTrailing() && msg.getMiddle().size() > 1)
		{
			_server->getChannelPtr(_target)->setTopic(msg.getMiddle().at(1));
			_server->getChannelPtr(_target)->sendToAll(":" + _buildPrefix(msg) + " TOPIC " + _target + " :"+ msg.getMiddle().at(1) + "\r\n");
		}
		else
			msg._client->reply(RPL_TOPIC(_target, _server->getChannelPtr(_target)->getTopic()));
	}
}
