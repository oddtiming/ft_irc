#include "commands/Mode.hpp"

Mode::Mode(Server* server) : Command("mode", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Mode::~Mode() {

}

/* Parse incoming mode request */
bool	Mode::parse(const Message& msg) {
	std::vector<std::string>	input = msg.getMiddle();

	/* Check if there is any target for command */
	if (input.empty() || input.at(0).empty())
		return false;
	
	std::vector<std::string>::const_iterator	it = input.begin();
	/* Get primary target */
	_target = *it++;

	/* Check if target is channel */
	if (_target.at(0) == '#')
		_targetType = CHANNEL;
	else
		_targetType = USER;
	
	/* Check for modes and add to _modes string */
	if (it != input.end())
		_modes = *it++;
	
	/* Check for secondary user target(s) for channel member modes */
	while (it != input.end())
		_params.push_back(*it++);

	return true;
}

/*******************************************/
/*            CONTROL FUNCTIONS            */
/*******************************************/

/* Function for executing indivdual modes */
bool	Mode::executeMode(char mode, bool removeMode) {
	switch(mode)
	{
		case 's'	:	return _secret(removeMode);
		case 't'	:	return _topic(removeMode);
		case 'i'	:	return _invite(removeMode);
		case 'n'	:	return _no_msg_in(removeMode);
		case 'k'	:	return _password(removeMode);
		case 'b'	:	return _ban(removeMode);
		case 'o'	:	return _operators(removeMode);
		default		:	_client->reply(ERR_UNKNOWNMODE(_server->getHostname(), _client->getNickname(), std::string(mode, 1)));
	}
	return false;
}

/* Attempt to execute command */
void	Mode::execute(const Message& msg) {
	clearAttributes();
	_client = msg._client;
	bool	removeMode = false;	// Is set to true if the mode is prepended with `-'

	/* Parse input to get targets and modes string */
	if (!parse(msg) || !validateTarget())
		return ;

	/* If there is only a target, then send modes list reply */
	if (_modes.empty())
	{
		/* Check if target exists */
		if (!validateTarget())
			return ;
		/* If target is channel, send list of channel modes for specified channel */
		if (_targetType == CHANNEL)
			sendChannelModes();
		/* If target is user, send list of global modes for specified user */
		else if (_targetType == USER)
			sendUserModes();
		return ;
	}

	/* Iterate through mode string and attempt to validate and execute each mode */
	std::string::iterator	ite = _modes.end();
	for (std::string::iterator it = _modes.begin(); it != ite; ++it)
	{
		if (*it == '-' || *it == '+')
			removeMode = (*it == '-') ? true : false;
		else if (!executeMode(*it, removeMode))
			continue ;
		_reply.append(1, *it);
	}

	if (_reply.find_first_not_of("+-") == std::string::npos)
		return ;

	/* Send reply message (if any) */
	if (_targetType == CHANNEL && _server->doesChannelExist(_target))
		_server->getChannelPtr(_target)->sendToAll(CMD_MODE(_buildPrefix(msg), _target, _reply));
	else if (_targetType == USER)
		_client->reply(CMD_MODE(_buildPrefix(msg), _target, _reply));
	// FIXME: RFC 2812 seems to suggest that the correct reply is RPL_UMODEIS (221),
	// but our tests so far suggest that the answer recognized by clients is what
	// corresponds to our CMD_MODE reply
}


/***************************************/
/*            CHANNEL MODES            */
/***************************************/

/* +s - Secret channel mode */
bool                Mode::_secret(bool removeMode)
{
	std::string mode("s");

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);

	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;

	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (secret).")), false);

	if (removeMode != channelPtr->checkModes(SECRET))
		return false;
	channelPtr->setModes(SECRET, removeMode);
	return true;
}

/* +t - topic limit channel mode (only OP can set topic) */
bool                Mode::_topic(bool removeMode)
{
	std::string mode("t");

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);
	
	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;

	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (topic).")), false);
	
	if (removeMode != channelPtr->checkModes(TOPIC_SET_OP))
		return false;
	channelPtr->setModes(TOPIC_SET_OP, removeMode);
	return true;
}

/* +i - invite only channel mode (must be invited to join channel) */
bool                Mode::_invite(bool removeMode)
{
	std::string mode("i");

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);
	
	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;
		
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (invite).")), false);

	if (removeMode != channelPtr->checkModes(INV_ONLY))
		return false;
	channelPtr->setModes(INV_ONLY, removeMode);
	return true;
}

/* +n - no message in channel mode (users that are not channel members can not send messages to channel) */
bool                Mode::_no_msg_in(bool removeMode)
{
	std::string mode("n");

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);

	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;
		
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (no_msg_in).")), false);

	if (removeMode != channelPtr->checkModes(NO_MSG_IN))
		return false;
	channelPtr->setModes(NO_MSG_IN, removeMode);
	return true;
}

/* +k - password required (channel is set to be password protected) */
bool                Mode::_password(bool removeMode)
{
	std::string	mode("k");
	std::string	pass;

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);
	
	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;
		
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (password protected).")), false);
	
	if (_params.empty())
		return (_client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), std::string("MODE +k"))), false);

	pass = _params.at(0);
	_params.erase(_params.begin());
	if (removeMode && pass.compare(channelPtr->getPassword()))
		return (_client->reply(ERR_KEYSET(_server->getHostname(), _client->getNickname(), _target)), false);

	if (removeMode != channelPtr->checkModes(PASS_REQ))
		return false;
	channelPtr->setModes(PASS_REQ, removeMode);
	if (!removeMode)
		channelPtr->setPassword(pass);
	return true;
}

/* +b - ban a user from channel, or return ban list for channel */
bool                Mode::_ban(bool removeMode)
{
	std::string	mode("b");
	std::string	targetNick;

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);
	
	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;
		
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (ban).")), false);
	
	if (_params.empty())
		return (sendBanList(), false);

	targetNick = _params.at(0);
	_params.erase(_params.begin());

	Client*	targetMember = _server->getClientPtr(targetNick);
	if (!targetMember)
		return (_client->reply(ERR_NOSUCHNICK(_server->getHostname(), _client->getNickname(), targetNick)), false);

	if (removeMode != channelPtr->checkMemberModes(targetMember, BAN))
		return false;
	channelPtr->setMemberModes(targetMember, BAN, removeMode);
	return true;
}

/* +o - give a user OP privs in channel */
bool                Mode::_operators(bool removeMode)
{
	std::string	mode("o");
	std::string	targetNick;

	if (_targetType == USER)
		return (_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), _client->getNickname(), mode)), false);
	
	Channel*	channelPtr = _server->getChannelPtr(_target);
	if (!channelPtr)
		return false;
		
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
		return (_client->reply(ERR_CHANOPRIVSNEEDED(_server->getHostname(), _client->getNickname(), _target, mode + " (operator).")), false);
	
	if (_params.empty())
		return (sendBanList(), false);

	targetNick = _params.at(0);
	_params.erase(_params.begin());

	Client*	targetMember = _server->getClientPtr(targetNick);
	if (!targetMember)
		return (_client->reply(ERR_NOSUCHNICK(_server->getHostname(), _client->getNickname(), targetNick)), false);

	if (removeMode != channelPtr->checkMemberModes(targetMember, C_OP))
		return false;
	channelPtr->setMemberModes(targetMember, C_OP, removeMode);
	return true;
}



/***********************************/
/*        Utility Functions        */
/***********************************/

/* Clear stored data */
void	Mode::clearAttributes(void)
{
	_target.clear();
	_params.clear();
	_modes.clear();
	_reply.clear();
	_targetType = 0;
	_client = nullptr;
}

/* Send list of modes for a given user */
void	Mode::sendUserModes(void) {
	/* Check permissions for sending requested mode info */
	if (_client->getNickname() != _target)
	{
		/* Reply error if target user is not client */
		_client->reply(ERR_USERSDONTMATCH(_server->getHostname(), _client->getNickname()));
		return ;
	}
	/* Reply list of global user modes if users match */
	_client->reply(RPL_UMODEIS(_server->getHostname(), _client->getNickname(), _client->getGlobalModes()));
}

/* Send a list of modes for a given channel */
void	Mode::sendChannelModes(void) {
	Channel* channel = _server->getChannelPtr(_target);
	
	/* Check permissions for sending requested mode info */
	if (channel->checkModes(SECRET) && !channel->isMember(_client))
	{
		_client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), _client->getNickname(), channel->getName()));
		return ;
	}
	_client->reply(RPL_CHANNELMODEIS(_server->getHostname(), _client->getNickname(), channel->getName(), channel->getChannelModes()));
}

/* Send a list of banned users from a given channel */
void	Mode::sendBanList(void) const {
	Channel* channel = _server->getChannelPtr(_target);
	if (!channel)
		return ;

	std::vector<std::string>			banList = channel->getBanList();
	std::vector<std::string>::iterator	ite = banList.end();
	for (std::vector<std::string>::iterator	it = banList.begin(); it != ite; ++it)
		_client->reply(RPL_BANLIST(_client->getNickname(), _target, *it));
	_client->reply(RPL_ENDOFBANLIST(_client->getNickname(), _target));
}

/* Check if target channel or user exists */
bool	Mode::validateTarget(void) {

	/* If target type is channel, check if channel exists */
	if (_targetType == CHANNEL && !_server->doesChannelExist(_target))
	{
		_client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), _client->getNickname(), _target));
		return false;
	}

	/* If target type is user, check if user exists*/
	else if (_targetType == USER && !_server->doesNickExist(_target))
	{
		_client->reply(ERR_NOSUCHNICK(_server->getHostname(), _client->getNickname(), _target));
		return false;
	}
	return (true);
}



/*
	NOTES::
	+imk is valid
	+i-m+k is also valid
	if the mode is already set then do nothing (no reply)
	/mode #channel b without sign requests channel ban list
	-m i  is valid and will set removemode for both


	Reply will contain all modes successfully changed in the order they were received (if mode was already set it will be removed from reply)
	Additionally another error for each mode that fails will be sent

	Irssi will not send /mode with no parameters - test with other clients. IF this is consistent then do nothing and ignore commands
*/