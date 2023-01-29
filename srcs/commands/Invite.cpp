#include "commands/Invite.hpp"

Invite::Invite(Server* server) : Command("invite", server) {
	_channelOpRequired = false;
	_globalOpRequired  = false;
}

Invite::~Invite( ) {}

bool Invite::validate(const Message& msg) {
	/* Ensure that there is both a target user and a target channel parameter */
	if (msg.getMiddle( ).size( ) < 2) {
		_client->reply(ERR_NEEDMOREPARAMS(
		  _server->getHostname( ), _client->getNickname( ), msg.getCommand( )));
		return false;
	}

	std::string nickname = msg.getMiddle( ).at(0);
	std::string channel  = msg.getMiddle( ).at(1);

	/* Check if target user exists */
	if (!_server->doesNickExist(nickname)) {
		_client->reply(
		  ERR_NOSUCHNICK(_server->getHostname( ), _client->getNickname( ), nickname));
		return false;
	}

	_targetUser = _server->getClientPtr(nickname);

	/* Check if channel exists */
	if (!_server->doesChannelExist(channel)) {
		_client->reply(
		  ERR_NOSUCHCHANNEL(_server->getHostname( ), _client->getNickname( ), channel));
		return false;
	}

	_targetChannel = _server->getChannelPtr(channel);

	/* Check if user attempting to send invite belongs to the target channel */
	if (!_targetChannel->isMember(_client)) {
		_client->reply(
		  ERR_NOTONCHANNEL(_server->getHostname( ), _client->getNickname( ), channel));
		return false;
	}

	/* Check if target user is already on target channel */
	if (_targetChannel->isMember(_targetUser)) {
		_client->reply(ERR_USERONCHANNEL(
		  _server->getHostname( ), _client->getNickname( ), nickname, channel));
		return false;
	}

	/* Check if target channel has +i flag (operator only invite) and user attempting to
	 * invite is not OP */
	if (_targetChannel->checkModes(INV_ONLY)
	    && !_targetChannel->checkMemberModes(_client, C_OP)) {
		_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname( ),
		                                    _client->getNickname( ),
		                                    _targetUser->getNickname( ),
		                                    "to send an invite."));
		return false;
	}

	return true;
}

void Invite::execute(const Message& msg) {
	_client = msg._client;

	if (validate(msg)) {
		/* Send message to target user */
		// NOTE: the prefix for this is the senders, not the receiver
		_targetUser->reply(CMD_INVITE(
		  _buildPrefix(msg), _targetUser->getNickname( ), _targetChannel->getName( )));

		/* Send message to client */
		_client->reply(RPL_INVITING(_server->getHostname( ),
		                            _client->getNickname( ),
		                            _targetUser->getNickname( ),
		                            _targetChannel->getName( )));

		/* If target user is away send reply to client */
		if (_targetUser->checkGlobalModes(AWAY))
			_client->reply(RPL_AWAY(_server->getHostname( ),
			                        _client->getNickname( ),
			                        _targetUser->getNickname( ),
			                        _targetUser->getAwayMessage( )));

		/* If channel was invite only, set invite flag for member */
		_targetChannel->setMemberModes(_targetUser, INVIT);
	}
}