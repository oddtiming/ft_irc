#include "commands/Notice.hpp"


Notice::Notice(Server* server) : Command("notice", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Notice::~Notice() {

}

bool	Notice::validate(const Message& msg) {
    std::vector<std::string>  args = msg.getMiddle();

    if(args.size() == 0 || _message.empty())
        return false;

    _target = args.at(0);
    args.erase(args.begin());
    if(_target.at(0) == '#')
    {
        _targetIsChannel = true;
        if(!_server->doesChannelExist(_target) || !_server->getChannelPtr(_target)->isMember(msg._client))
			return false;
		return true;
    }
	_targetIsChannel = false;
	if (!_server->doesNickExist(_target))
		return false;
    return true;
}

void	Notice::execute(const Message& msg)
{
	_buildMessage(msg);

	if (!validate(msg))
		return ;

	if (_targetIsChannel)
		_server->getChannelPtr(_target)->sendToOthers(
			CMD_NOTICE(_buildPrefix(msg), _target, _message), msg._client);
	else
		_server->getClientPtr(_target)->reply(
			CMD_NOTICE(_buildPrefix(msg), _target, _message));
}

void	Notice::_buildMessage(const Message& msg)
{
	size_t	nb_args = msg.getMiddle().size();
	
	/* If the message was a single word, some clients (e.g. Limechat) do not
		prepend a ':' before it, so it stays in the msg's _middle field */
	for (size_t i = 2; i < nb_args; ++i)
		_message.append(msg.getMiddle().at(i));
	_message.append(msg.getTrailing());
}
