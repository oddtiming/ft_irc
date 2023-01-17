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
	if (middle.empty())
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
	/*check if user is on the channel*/
	if (!_server->getChannelPtr(channel)->isMember(_client->getNickname())){
		msg._client->reply(ERR_NOTONCHANNEL(channel));
		std::cerr << "ERR_NOTONCHANNEL" << std::endl;
		return false;
	}
	/*check if target is a member of the channel*/
	if (!_server->getChannelPtr(channel)->isMember(nick)){
		msg._client->reply(ERR_NOSUCHNICK(nick));
		std::cerr << "ERR_NOSUCHNICK" << std::endl;
		return false;
	}
	//TODO: check if user has op priv

}

void	Kick::execute(const Message& msg) {
	if (validate(msg)) {
		std::string					channel = middle.at(0);
		std::string					user = middle.at(1);
		std::string	message = "KICK " + chan->getName() + " " + msg.getParams()[1] + " :";

		if (msg.getMiddle().size() > 2)
			message += msg.getParams()[2];
		else
			message += _user.getNickname();
		msg._client->reply(message);
		//TODO: add code to kick user from channel
	}

}
