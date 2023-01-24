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
	if (input.empty())
		return false;
	
	/* Get primary target */
	_target = input.at(0);

	/* Check if target is channel */
	if (input.at(0).at(0) == '#')
		_targetType = CHANNEL;
	else
		_targetType = USER;
	
	/* Check for modes and add to _modes string */
	if (input.size() > 1)
		_modes = input.at(1);
	
	/* Check for secondary user target for channel member modes */
	if (input.size() > 2)
		_targetMember = input.at(2);

	return true;
}


bool	Mode::validate(const Message& msg) {
	std::vector<std::string> middle = msg.getMiddle();


	/*check if there's a target for the command*/
	//fixme: when receiving "MODE #channelName", reply with channel modes
	if (middle.size() < 1) {
		msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
		return false;
	}
	std::string target = middle.at(0);
	bool removeMode;
	std::string modes = "+-";


	if (target.at(0) == '#')
		_targetIsChannel = 1;
	else
		_targetIsChannel = 0;
	if (middle.size() == 1 && _targetIsChannel)
	{
		msg._client->reply(RPL_CHANNELMODEIS(target, "+", _server->getChannelPtr(target)->getChannelModes()));
		return false;
	}
	//fixme: add code to send all modes to client in case of /mode #targetChannel
	if (middle.size() > 1) {
		std::string mode = middle.at(1);
		if (mode.c_str()[0] == '-')
			removeMode = true;
		else if (mode.c_str()[0] == '+')
			removeMode = false;
		else {
			msg._client->reply(ERR_UNKNOWNMODE(mode, target));
			return false;
		}
		if (_targetIsChannel == 0){
			if (mode.c_str()[1] == 'o') {
				msg._client->setGlobalModes(OP, removeMode);
				msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
			} else if (mode.c_str()[1] == 'i') {
				msg._client->setGlobalModes(INVIS, removeMode);
				msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
			} else {
				msg._client->reply(ERR_UNKNOWNMODE(mode, target));
				return false;
			}
		}
		if (_targetIsChannel ==
			1/* && (msg._client->checkGlobalModes(OP) || _server->getChannelPtr(target)->checkMemberModes(msg._client,C_OP ))*/) {
			{

				if (mode.c_str()[1] == 'p') {
					_server->getChannelPtr(target)->setModes(PRIVATE, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 's') {
					_server->getChannelPtr(target)->setModes(SECRET, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'm') {
					_server->getChannelPtr(target)->setModes(MODERATED, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'i') {
					_server->getChannelPtr(target)->setModes(INV_ONLY, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 't') {
					_server->getChannelPtr(target)->setModes(TOPIC_SET_OP, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'n') {
					_server->getChannelPtr(target)->setModes(NO_MSG_IN, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'k') {
					_server->getChannelPtr(target)->setModes(PASS_REQ, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}

				if (middle.size() > 2) {
					std::string user = middle.at(2);
					if (mode.c_str()[1] == 'b') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), BAN, removeMode);
						if (_server->getChannelPtr(target)->isMember(_server->getClientPtr(user)))
							_server->getChannelPtr(target)->removeMember(_server->getClientPtr(user),
																		 CMD_PART(user, target, "User has been banned"));
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
					if (mode.c_str()[1] == 'i') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), INV, removeMode);
						// msg._client->reply(RPL_INVITING(target, user));
					}
					if (mode.c_str()[1] == 'v') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), VOICE, removeMode);
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
					if (mode.c_str()[1] == 'o') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), C_OP, removeMode);
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
				}
				else {
					msg._client->reply(ERR_UNKNOWNMODE(mode, target));
					return false;
				}
			}

		}
	}
	return true;
}

/* Function to clear stored data before returning */
void	Mode::clearData(void) {
	_target.clear();
	_targetMember.clear();
	_modes.clear();
	_targetType = 0;
	_reply.clear();
	_client = nullptr;
}

/* Send list of modes for a given user */
void	Mode::sendUserModes() {
	
}

/* Send a list of modes for a given channel */
void	Mode::sendChannelModes() {

}

/* Attempt to execute command */
void	Mode::execute(const Message& msg) {
	bool	removeMode = false;	// Is set to true if the mode is prepended with `-'
	
	_client = msg._client;

	/* Parse input to get targets and modes string */
	if (!parse(msg))
	{
		clearData();
		return ;
	}

	/* If there is only a target, then send modes list reply */
	if (_modes.empty())
	{
		if (_targetType == CHANNEL)
			sendChannelModes();
		else
			sendUserModes();
	}

	std::string::iterator	ite = _modes.end();
	for (std::string::iterator it = _modes.begin(); it != ite; ++it)
	{
		if (*it == '-' || *it == '+')
			removeMode = (*it == '-') ? true : false;
		
		/* Iterate through mode string and attempt to validate each individual modes */
		// FIXME: validate needs to be completely overhauled
		// 			validate_mode(char mode, removeMode)
		// NOTE: it also needs to send the numerical replies 
		else if (!validate(msg))
			continue ;
		_reply.append(*it);
	}



	/* Send reply message (if any) */
	
	/* Channel mode messages are sent to all members of channel */
	/* Member mode message are sent to all member of channel */
	
	/* Clear all stored data before returning */
	clearData();
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