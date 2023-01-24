#include "../../includes/commands/Whois.hpp"

Whois::Whois(Server* server) : Command("whois", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Whois::~Whois() {

}

bool	Whois::validate(const Message& msg) {
	if (!msg.hasMiddle()) {
		msg._client->reply(ERR_NONICKNAMEGIVEN());
		return false;
	}
	std::string target = msg.getMiddle().at(0);

	if (!_server->doesNickExist(target)) {
		msg._client->reply(ERR_NOSUCHNICK(target));
		return false;
	}
	_target = _server->getClientPtr(target);

	return true;
}

void	Whois::execute(const Message& msg) {

	if (validate(msg))
	{
			std::string nick = _target->getNickname();
			std::string user = _target->getUsername();
			std::string host = _target->getHostname();
			std::string realName = _target->getRealname();
			msg._client->reply(RPL_WHOISUSER(nick, user, host, realName));
			/* Iterate over every channel to see which the target user is part of*/
			ChannelList channelList = _server->getChannelList();
			ChannelList::iterator it = channelList.begin();
			ChannelList::iterator ite = channelList.end();
			for (; it != ite; ++it)
			{
				std::string reply = _target->getNickname() + " : ";
				if (it->second->isMember(_target) && (!it->second->checkModes(SECRET) ||it->second->isMember(msg._client))) {
					if (it->second->checkMemberModes(_target, C_OP))
						reply += "@" + it->first;
					else
						reply += it->first;
					msg._client->reply(RPL_WHOISCHANNELS(reply));
				}
			}
			msg._client->reply(RPL_WHOISSERVER(_target->getNickname(), "ircserv", _server->getHostname()));
			msg._client->reply(RPL_WHOISIDLE(_target->getNickname(), std::to_string(std::time(nullptr) - _target->getLastActivityTime())));
			msg._client->reply(RPL_AWAY(_target->getHostname(), msg._client->getNickname(), _target->getNickname(), _target->getAwayMessage()));
			msg._client->reply(RPL_ENDOFWHOIS(_target->getNickname()));



	}

}