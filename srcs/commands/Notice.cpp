#include "commands/Notice.hpp"


Notice::Notice(Server* server) : Command("notice", server) { }

/* Validate if command can be executed */
bool	Notice::validate(const Message& msg) {

	/* If no target parameter return false*/
	if (!msg.hasMiddle())
		return false;

	_target = msg.getMiddle().at(0);

	/* If target is a channel */
    if(_target.at(0) == '#')
    {
        _targetIsChannel = true;
		
		/* Check if channel exists */
		if (!_server->doesChannelExist(_target))
			return false;
		
		_channel = _server->getChannelPtr(_target);
		/* Check if channel has +n flag and client is not a member */
		if (!_channel->isMember(_client) && _channel->checkModes(NO_MSG_IN))
			return false;
    }
	/* Check if user target exists */
	else if (!_server->doesNickExist(_target))
		return false;
    return true;
}

/* Clear data from previous function calls */
void	Notice::clearData() {
	_client = nullptr;
	_channel = nullptr;
	_target.clear();
    _message.clear();
	_targetIsChannel = false;
}

/* Attempt to validate and execute command */
void	Notice::execute(const Message& msg) {
	clearData();
	_client = msg._client;

	if (!validate(msg))
		return ;
	_message = msg.getTrailing();

	if (_targetIsChannel)
		_channel->sendToOthers(CMD_NOTICE(_buildPrefix(msg), _target, _message), _client);
	else
		_server->getClientPtr(_target)->reply(CMD_NOTICE(_buildPrefix(msg), _target, _message));
}