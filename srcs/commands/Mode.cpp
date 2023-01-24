#include "commands/Mode.hpp"

Mode::Mode(Server* server) : Command("mode", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Mode::~Mode() {

}

/* Parse incoming mode request */
bool	Mode::parse(const Message& msg) {
	std::vector<std::string>					input = msg.getMiddle();

	/* Check if there is any target for command */
	if (input.empty() || input.at(0).empty())
		return false;
	
	std::vector<std::string>::const_iterator	it = input.begin();
	/* Get primary target */
	_target = *it;

	/* Check if target is channel */
	if (_target.at(0) == '#')
		_targetType = CHANNEL;
	else
		_targetType = USER;
	
	/* Check for modes and add to _modes string */
	if (++it != input.end())
		_modes = *it;
	
	/* Check for secondary user target for channel member modes */
	if (++it != input.end())
		_params.insert(_params.end(), it, _input.end());

	return true;
}

/*******************************************/
/*            CONTROL FUNCTIONS            */
/*******************************************/

/* Function for executing indivdual modes */
bool	Mode::executeMode(char mode, bool removeMode) const {
	switch(mode)
	{
		case 's'	:	return _secret();
		case 't'	:	return _topic();
		case 'i'	:	return _invite();
		case 'n'	:	return _no_msg_in();
		case 'k'	:	return _password();
		case 'b'	:	return _ban();
		case 'o'	:	return _operators();
		default		:	_client.reply(ERR_UNKNOWNMODE(mode, _target));
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
		_reply.append(*it);
	}

	if (_reply.find_first_not_of("+-") == std::string::npos)
		return ;

	/* Send reply message (if any) */
	if (_targetType == CHANNEL)
		_server->getChannelPtr()->sendToAll(CMD_MODE(_buildPrefix(msg), _target, _reply));
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
bool                Mode::_secret(bool removeMode) const
{
	char mode = 's';

	// If the target is not a channel, treat 's' as unknown user mode
	if (_targetType != CHANNEL)
	{
		_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), mode + " (secret)", _target));
		return false;
	}
	Channel*	channelPtr = _server.getChannelPtr(_target);
	if (!channelPtr)
		return false;
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
	{
		_client.reply(ERR_CHANOPRIVSNEEDED(_target, mode + " (secret)."));
		return false;
	}
	channelPtr->setModes(SECRET, removeMode);
	return true;
}

/* +t - topic limit channel mode (only OP can set topic) */
bool                Mode::_topic(bool removeMode) const
{
	char mode = 't';

	// If the target is not a channel, treat 't' as unknown user mode
	if (_targetType != CHANNEL)
	{
		_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), mode + " (topic)", _target));
		return false;
	}
	Channel*	channelPtr = _server.getChannelPtr(_target);
	if (!channelPtr)
		return false;
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
	{
		_client.reply(ERR_CHANOPRIVSNEEDED(_target, mode + " (topic)."));
		return false;
	}
	channelPtr->setModes(TOPIC_SET_OP, removeMode);
	return true;
}

/* +i - invite only channel mode (must be invited to join channel) */
bool                Mode::_invite(bool removeMode) const
{
	char mode = 'i';

	// If the target is not a channel, treat 'i' as unknown user mode
	if (_targetType != CHANNEL)
	{
		_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), mode + " (invite)", _target));
		return false;
	}
	Channel*	channelPtr = _server.getChannelPtr(_target);
	if (!channelPtr)
		return false;
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
	{
		_client.reply(ERR_CHANOPRIVSNEEDED(_target, mode + " (invite)."));
		return false;
	}
	channelPtr->setModes(INV_ONLY, removeMode);
	return true;
}

/* +n - no message in channel mode (users that are not channel members can not send messages to channel) */
bool                Mode::_no_msg_in(bool removeMode) const
{
	char mode = 'n';

	// If the target is not a channel, treat 'n' as unknown user mode
	if (_targetType != CHANNEL)
	{
		_client->reply(ERR_UMODEUNKNOWNFLAG(_server->getHostname(), mode + " (no_msg_in)", _target));
		return false;
	}
	Channel*	channelPtr = _server.getChannelPtr(_target);
	if (!channelPtr)
		return false;
	if (channelPtr->checkMemberModes(_client, C_OP | OWNER))
	{
		_client.reply(ERR_CHANOPRIVSNEEDED(_target, mode + " (no_msg_in)."));
		return false;
	}
	channelPtr->setModes(NO_MSG_IN, removeMode);
	return true;
}

/* +k - password required (channel is set to be password protected) */
bool                Mode::_password(bool removeMode) const
{
	return true;
}

/* +b - ban a user from channel, or return ban list for channel */
bool                Mode::_ban(bool removeMode) const
{
	return true;
}

/* +o - give a user OP privs in channel */
bool                Mode::_operators(bool removeMode) const
{
	return true;
}



/***********************************/
/*        Utility Functions        */
/***********************************/

/* Clear stored data */
void	Mode::clearAttributes(void) {
	_target.clear();
	_params.clear();
	_modes.clear();
	_targetType = 0;
	_reply.clear();
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
	_client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
}

/* Send a list of modes for a given channel */
void	Mode::sendChannelModes(void) {
	Channel* channel = _server->getChannelPtr(_target);
	
	/* Check permissions for sending requested mode info */
	if (channel->checkModes(SECRET) && !channel->isMember(_client->getNickname()))
	{
		_client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), _client->getNickname(), channel->getName()));
		return ;
	}
	_client->reply(RPL_CHANNELMODEIS(_server->getHostname(), _client->getNickname(), channel->getName(), channel->getChannelModes()));
}

/* Check if target channel or user exists */
bool	Mode::validateTarget(void) {

	/* If target type is channel, check if channel exists */
	if (_targetType == CHANNEL && !_server.doesChannelExist(_target))
	{
		_client.reply(ERR_NOSUCHCHAN());
		return false;
	}

	/* If target type is user, check if user exists*/
	else if (_targetType == USER && !_server.doesNickExist(_target))
	{
		_client.reply(ERR_NOSUCHNICK());
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