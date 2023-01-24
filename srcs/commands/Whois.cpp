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
	_target = msg.getMiddle().at(0);


	if (!_server->doesNickExist(_target) || _server->getClientPtr(_target)->checkGlobalModes(INVIS)) {
		msg._client->reply(ERR_NOSUCHNICK(_target));
		return false;
	}

	return true;
}

void	Whois::execute(const Message& msg) {

	if (validate(msg))
	{
			std::string nick = _server->getClientPtr(_target)->getNickname();
			std::string user = _server->getClientPtr(_target)->getUsername();
			std::string host = _server->getClientPtr(_target)->getHostname();
			std::string realName = _server->getClientPtr(_target)->getRealname();
			msg._client->reply(RPL_WHOISUSER(nick, user, host, realName));

			std::map<std::string, Channel *> channelList = _server->getChannelList();
			std::map<std::string, Channel *>::iterator it = channelList.begin();
			std::map<std::string, Channel *>::iterator ite = channelList.end();
			for (; it != ite; ++it)
			{
				std::string reply = _target + " :* ";
				if (it->second->isMember(_server->getClientPtr(_target))) {
					if (it->second->checkMemberModes(_server->getClientPtr(_target), C_OP))
						reply += "@" + it->first;
					else
						reply += it->first;
					msg._client->reply(RPL_WHOISCHANNELS(reply));
				}
			}
			msg._client->reply(RPL_ENDOFWHOIS(_target));



	}

}