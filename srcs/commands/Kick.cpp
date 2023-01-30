#include "commands/Kick.hpp"

Kick::Kick(Server* server) : Command("kick", server) { }

bool	Kick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();

	/* Check if there's a target for the command */
	if (middle.size() < 2)
	{
		_client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
		return false;
	}
	_targetChannel = middle.at(0);
	_targetUser = middle.at(1);


	/* Check if channel exists */
	if (!_server->doesChannelExist(_targetChannel)) {
		_client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), _client->getNickname(), _targetChannel));
		return false;
	}
	_channel = _server->getChannelPtr(_targetChannel);

	/* Check if client has OP privs for specified channel */
	if (!_channel->checkMemberModes(_client, C_OP)) {
		_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _targetChannel, "kick"));
		return false;
	}

	/* Check if client is a member of the specified channel */
	if (!_channel->isMember(_client)) {
		_client->reply(ERR_NOTONCHANNEL(_server->getHostname(), _client->getNickname(), _targetChannel));
		return false;
	}

	/* Check if target is a member of the specified channel */
	if (!_channel->isMember(_server->getClientPtr(_targetUser))) {
		_client->reply(ERR_NOSUCHNICK(_server->getHostname(), _client->getNickname(), _targetUser));
		return false;
	}
	return true;
}

/* Clear all data from previous function calls */
void	Kick::clearData() {
	_client = nullptr;
	_channel = nullptr;
	_targetChannel.clear();
	_targetUser.clear();
	_message.clear();
}

void	Kick::execute(const Message& msg) {
	/* Ensure that all data from previous function calls is removed */
	clearData();
	_client = msg._client;
	if (validate(msg)) {
		
		/* If a kick message was provided then add to reply */
		_message = msg.getTrailing();
	
		/* Send message to all channel members */
		_channel->sendToAll(CMD_KICK(_buildPrefix(msg), _targetChannel, _targetUser, _message));

		/* Remove kicked user from channel */
		_channel->removeMember(_server->getClientPtr(_targetUser));

		/* If channel is empty, delete it */
		if (_channel->getIsEmpty())
			_server->destroyChannel(_targetChannel);
	}
}
