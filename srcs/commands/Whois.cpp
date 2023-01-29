#include "../../includes/commands/Whois.hpp"

Whois::Whois(Server* server) : Command("whois", server) {
	_channelOpRequired = false;
	_globalOpRequired  = false;
}

Whois::~Whois( ) {}

bool Whois::validate(const Message& msg) {
	if (!msg.hasMiddle( )) {
		msg._client->reply(ERR_NONICKNAMEGIVEN(_server->getHostname( )));
		return false;
	}
	std::string target = msg.getMiddle( ).at(0);

	if (!_server->doesNickExist(target)) {
		msg._client->reply(
		  ERR_NOSUCHNICK(_server->getHostname( ), _client->getNickname( ), target));
		return false;
	}
	_target = _server->getClientPtr(target);

	return true;
}

void Whois::execute(const Message& msg) {
	_client = msg._client;
	if (validate(msg)) {
		std::string host    = _server->getHostname( );
		std::string nick    = _target->getNickname( );
		std::string user    = _target->getUsername( );
		std::string address = _target->getAddress( );
		std::string real    = _target->getRealname( );
		msg._client->reply(
		  RPL_WHOISUSER(host, _client->getNickname( ), nick, user, address, real));

		/* Iterate over every channel to see which the target user is part of */
		ChannelList           channelList = _server->getChannelList( );
		ChannelList::iterator it          = channelList.begin( );
		ChannelList::iterator ite         = channelList.end( );
		bool                  hasTargets  = false;

		std::string reply;
		for (; it != ite; ++it) {
			/* If both querying and target users share the channel, target is not set as
			 * invisible, and channel is not set as secret, send RPL_WHOISCHANNELS */
			if (it->second->isMember(_target) && it->second->isMember(msg._client)
			    && !_target->checkGlobalModes(INVIS) && !it->second->checkModes(SECRET)) {
				if (hasTargets)
					reply += ",";
				if (it->second->checkMemberModes(_target, C_OP))
					reply += "@" + it->first;
				else
					reply += it->first;
				hasTargets = true;
			}
		}
		if (hasTargets)
			msg._client->reply(RPL_WHOISCHANNELS(_server->getHostname( ),
			                                     _client->getNickname( ),
			                                     _target->getNickname( ),
			                                     reply));
		msg._client->reply(RPL_WHOISSERVER(_server->getHostname( ),
		                                   _client->getNickname( ),
		                                   _target->getNickname( ),
		                                   _server->getServername( )));
		msg._client->reply(RPL_WHOISIDLE(
		  _server->getHostname( ),
		  _client->getNickname( ),
		  _target->getNickname( ),
		  std::to_string(std::time(nullptr) - _target->getLastActivityTime( )),
		  std::to_string(std::time(nullptr) - _target->getConnectTime( ))));
		msg._client->reply(RPL_AWAY(_target->getHostname( ),
		                            msg._client->getNickname( ),
		                            _target->getNickname( ),
		                            _target->getAwayMessage( )));
		msg._client->reply(RPL_ENDOFWHOIS(
		  _server->getHostname( ), _client->getNickname( ), _target->getNickname( )));
	}
}