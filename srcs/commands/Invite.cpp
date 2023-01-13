#include "commands/Invite.hpp"

Invite::Invite(Server* server) : Command("invite", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Invite::~Invite() {

}

bool	Invite::validate(const Message& msg) {
	if (getMiddle().size() < 2)
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
		return false;
	}
	std::string					nickname = getMiddle().at(0);
	std::string					channel = getMiddle().at(1);
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
	/*check if target exists*/
	if (!_server->doesNickExist(nickname)){
		msg._client->reply(ERR_NOSUCHNICK(nickname));
		std::cerr << "ERR_NOSUCHNICK" << std::endl;
		return false;
	}
	//TODO: Add permission check for invitation (+o +i, also away status)
}

void	Invite::execute(const Message& msg) {

	if (validate(msg))
	{
		//TODO add invitation message to user
	}

}