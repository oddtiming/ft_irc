#include "commands/Notice.hpp"


Notice::Notice(Server* server) : Command("notice", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Notice::~Notice() {

}

bool	Notice::validate(const Message& msg) {
	std::vector<std::string>  args = msg.getMiddle();

	if(args.size() == 0)
		return false;
	if (args.size() == 1)
		return false;
	_target = args.at(0);
	args.erase(args.begin());
	if(_target.at(0) == '#')
	{
		_targetIsChannel = true;
		if(!(_server->doesChannelExist(_target)) ||
		   		!(_server->isUserChannelMember(msg._client->getNickname()) ||
			 	!(msg._client->checkChannelModes(_target, BAN | INV_ONLY))))
			return false;
	}
	else
	{
		_targetIsChannel = false;
		if (msg._client->checkGlobalMode(AWAY))
			return false;
		if (!(_server.doesNickExist(_target)))
			return false;
	}
	return true;
}

void	Notice::execute(const Message& msg) {
	if (validate(msg)) {
		if (_targetIsChannel)
			//FIXME: sendto function to be added in order to message user/channel
		else
			//send to target
	}
}