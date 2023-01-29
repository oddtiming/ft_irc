#include "commands/Kick.hpp"

Kick::Kick(Server* server) : Command("kick", server) {
	_channelOpRequired = true;
	_globalOpRequired = false;
}


Kick::~Kick() {

}

bool	Kick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();

	/*check if there's a target for the command*/
	if (middle.size() < 2)
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
		return false;
	}
	std::string					channel = middle.at(0);
	std::string					user = middle.at(1);
	/*check if channel exists*/
	if (!_server->getChannelPtr(channel)->checkMemberModes(msg._client, C_OP)) {
		msg._client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), channel, "kick"));
		return false;
	}
	if (!_server->doesChannelExist(channel)){
		msg._client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), msg._client->getNickname(), channel));
		return false;
	}
	/*check if target user is on the channel*/
	if (!_server->getChannelPtr(channel)->isMember(msg._client)){
		msg._client->reply(ERR_NOTONCHANNEL(_server->getHostname(), _client->getNickname(), channel));
		return false;
	}
	/*check if target is a member of the channel*/
	if (!_server->getChannelPtr(channel)->isMember(_server->getClientPtr(user))){
		msg._client->reply(ERR_NOSUCHNICK(_server->getHostname(), _client->getNickname(), user));
		return false;
	}
	return true;
}

void	Kick::execute(const Message& msg) {
	if (validate(msg)) {
		_client = msg._client;
		std::string channel = msg.getMiddle().at(0);
		std::string user = msg.getMiddle().at(1);
		std::string message;
		Channel* channelPtr = _server->getChannelPtr(channel);

		if (msg.getMiddle().size() > 2)
			message = msg.getMiddle().at(2);
		else
			message = msg.getTrailing();
		/*Sends a kick message to every user on the channel then remove target member*/
		channelPtr->sendToAll(":" + _buildPrefix(msg) + " KICK " + channel + " " + user + " :" +
							  message + "\r\n");
		channelPtr->removeMember(_server->getClientPtr(user));
		
		/* If channel is empty, delete it */
		if (channelPtr->getIsEmpty())
			_server->destroyChannel(channel);
	}
}
