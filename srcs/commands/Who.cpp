#include "commands/Who.hpp"

Who::Who(Server* server) : Command("who", server) {
	_channelOpRequired = false;
	_globalOpRequired  = false;
}

Who::~Who( ) {}


bool Who::validate(const Message& msg) {
	_target = nullptr;
	/* If no target, will list all users with no common channels */
	if (!msg.hasMiddle( ) || msg.getMiddle( ).at(0).empty( )) {
		return false;
	}

	std::string target = msg.getMiddle( ).at(0);
	/* If target is channel, will list all users on that channel */
	if (!target.empty( ) && target.at(0) == '#') {
		_targetType = CHANNEL;
		_target     = static_cast< void* >(_server->getChannelPtr(target));
		if (!_target) {
			_client->reply(ERR_NOSUCHCHANNEL(
			  _server->getHostname( ), _client->getNickname( ), target));
			return false;
		}
		return true;
	}

	/* If target is client, will list all channels client is on */
	if (!_server->doesNickExist(target)) {
		msg._client->reply(
		  ERR_NOSUCHNICK(_server->getHostname( ), _client->getNickname( ), target));
		return false;
	}
	_targetType = USER;
	_target = static_cast< void* >(_server->getClientPtr(target));
	return true;
}

void Who::execute(const Message& msg) {
	_client = msg._client;
	/* Check permissions for execution of function */
	if (validate(msg)) {
		if (_targetType == CHANNEL)
			return _listChannelMembers( );
		if (_targetType == USER)
			return _listClientChannels( );
	}
}

void Who::_listChannelMembers( ) {
	Channel* targetChannel = static_cast< Channel* >(_target);
	if (!targetChannel)
		return;
	std::string members = targetChannel->getMemberList(targetChannel->isMember(_client));
	bool        targetIsOpe;
	size_t      it = members.find(" ");

	// Split the space-separated list of members
	while ((it = members.find(" ")) != std::string::npos) {
		std::string currMember = members.substr(0, it);
		members                = members.substr(it + 1, members.size( ));
		targetIsOpe            = false;
		if (!currMember.empty( ) && currMember.at(0) == '@') {
			targetIsOpe = true;
			currMember.erase(0, 1);
		}
		Client* currMemberPtr = _server->getClientPtr(currMember);
		if (!currMemberPtr) {
			std::cerr << "Error retrieving client ptr for '" << currMember << "'."
			          << std::endl;
			continue;
		}
		// For each channel member, send a RPL_WHOREPLY with their status on the channel
		if (currMemberPtr->checkGlobalModes(AWAY))
			currMember.append(" G");
		else
			currMember.append(" H");
		if (targetIsOpe)
			currMember.append("@");

		// Sending currMember as <nick> field, because irssi seems to parse it better
		_client->reply(RPL_WHOREPLY(_server->getHostname( ),
		                            targetChannel->getName( ),
		                            currMember,
		                            _client->getUsername( ),
		                            currMember,
		                            currMemberPtr->getRealname( )));
	}
	_client->reply(RPL_ENDOFWHO(
	  _server->getHostname( ), _client->getNickname( ), targetChannel->getName( )));
}

void Who::_listClientChannels( ) {
	Client* targetMember = static_cast< Client* >(_target);
	if (!targetMember)
		return;
	// If the queried client is set as invisible, do not send its status
	if (targetMember->checkGlobalModes(INVIS)) {
		_client->reply(RPL_ENDOFWHO(_server->getHostname( ),
		                           _client->getNickname( ),
		                           targetMember->getNickname( )));
		return;
	}
	ChannelList           channels = _server->getChannelList( );
	ChannelList::iterator ite      = channels.end( );
	std::string	currMember = targetMember->getNickname();
	if (targetMember->checkGlobalModes(AWAY))
		currMember.append(" G");
	else
		currMember.append(" H");
	// For each channel of which client is a member, send a RPL_WHOREPLY with their status on channel
	for (ChannelList::iterator it = channels.begin( ); it != ite; ++it) {
		if (*(currMember.end() - 1) == '@')
			currMember.erase(currMember.end() - 1, currMember.end());
		if (it->second->isMember(targetMember)) {
			if (it->second->checkMemberModes(targetMember, C_OP | OWNER))
				currMember.append("@");
			_client->reply(RPL_WHOREPLY(_server->getHostname( ),
			                            targetMember->getNickname( ),
			                            it->first,
			                            _client->getUsername( ),
			                            currMember,
			                            targetMember->getRealname( )));
		}
	}
	_client->reply(RPL_ENDOFWHO(
	  _server->getHostname( ), _client->getNickname( ), targetMember->getNickname( )));
}
