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
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
		return false;
	}
	std::string					channel = middle.at(0);
	std::string					user = middle.at(1);
	/*check if channel exists*/
	if (!_server->doesChannelExist(channel)){
		msg._client->reply(ERR_NOSUCHCHANNEL(channel));
		std::cerr << "ERR_NOSUCHCHANNEL" << std::endl;
		return false;
	}
	/*check if target user is on the channel*/
	if (!_server->getChannelPtr(channel)->isMember(msg._client)){
		msg._client->reply(ERR_NOTONCHANNEL(channel));
		std::cerr << "ERR_NOTONCHANNEL" << std::endl;
		return false;
	}
	/*check if target is a member of the channel*/
	if (!_server->getChannelPtr(channel)->isMember(_server->getClientPtr(user))){
		msg._client->reply(ERR_NOSUCHNICK(user));
		std::cerr << "ERR_NOSUCHNICK" << std::endl;
		return false;
	}
	//TODO: check if user has op priv

}

void	Kick::execute(const Message& msg) {
	if (validate(msg)) {
		std::string					channel = msg.getMiddle().at(0);
		std::string					user = msg.getMiddle().at(1);
		std::string	message = "KICK " + channel + " " + user + " :";

		if (!msg.getTrailing().empty())
			message += msg.getTrailing();
		else
			message += msg._client->getNickname();
		msg._client->reply(message);
		//TODO: add code to kick user from channel
	}

}
