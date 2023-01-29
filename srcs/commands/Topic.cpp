#include "../../includes/commands/Topic.hpp"

Topic::Topic(Server* server) : Command("topic", server) { }

/* Attempt to validate if command can be executed */
bool	Topic::validate(const Message& msg) {

	/* Check if any parameters were passed */
	if (!msg.hasMiddle())
	{
		_client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
		return false;
	}
	_target = msg.getMiddle().at(0);
	
	/* Check if target channel exists */
	if (!_server->doesChannelExist(_target))
	{
		_client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), msg._client->getNickname(), _target));
		return false;
	}
	_channel = _server->getChannelPtr(_target);

	/* Check if client is a member of the target channel */
	if (!_channel->isMember(_client)) {
		_client->reply(ERR_NOTONCHANNEL(_server->getHostname(), _client->getNickname(), _target));
		return false;
	}

	/* If +t flag is set, check is client has OP privs on target channel */
	if (_channel->checkModes(TOPIC_SET_OP) && !_channel->checkMemberModes(_client, C_OP)) {
		_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, 't'));
		return false;
	}
	return true;
}

/* Remove data from previous function calls */
void	Topic::clearData() {
	_client = nullptr;
	_channel = nullptr;
	_target.clear();
	_topic.clear();
}

/* Attempt to validate and execute command */
void	Topic::execute(const Message& msg) {
	/* Clear data from previous function calls */
	clearData();
	_client = msg._client;


	if (validate(msg))
	{
		/* If message has trailing but trailing is empty, clear topic */
		if (msg.hasTrailing() && msg.getTrailing().empty())
		{
			_channel->setTopic("");
			_channel->sendToAll(":" + _buildPrefix(msg) + " TOPIC " + _target + " :" + "\r\n");
		}
		/* If message has trailing and trailing is not empty, set the topic */
		else if (msg.hasTrailing() && !msg.getTrailing().empty())
		{
			_channel->setTopic(msg.getTrailing());
			_channel->sendToAll(":" + _buildPrefix(msg) + " TOPIC " + _target + " :"+ msg.getTrailing() + "\r\n");
		}
		/* If message does not have trailing, send the current topic */
		else
		{
			_topic = _channel->getTopic();
			if (_topic.empty())
				_client->reply(RPL_NOTOPIC(_server->getHostname(), _client->getNickname(), _target));
			else
				_client->reply(RPL_TOPIC(_server->getHostname(), _client->getNickname(), _target, _topic));
		}
	}
}