#include "../../includes/commands/Names.hpp"

Names::Names(Server* server) : Command("names", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Names::~Names() {

}

bool	Names::validate(const Message& msg) {
	if (msg.hasMiddle())
	{
		if (msg.hasMiddle() > 0 && msg.getMiddle().at(0).size() > 0) {
			if (_server->doesChannelExist(msg.getMiddle().at(0))) {
				_target = msg.getMiddle().at(0);
				_hasTarget = true;
			}
			else
				return false;
		}
	}

	return true;

}

void	Names::execute(const Message& msg) {

	if (validate(msg)) {
		if (_hasTarget)
		{
				msg._client->reply(RPL_NAMREPLY(_server->getHostname(), msg._client->getNickname(), _target, _server->getChannelPtr(_target)->getMemberList()));
				msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), _target));
		}
		else
		{
			ChannelList channelList = _server->getChannelList();
			ChannelList::iterator itC = channelList.begin();
			ChannelList::iterator iteC = channelList.end();
			for (; itC != iteC; ++itC)
			{
				if (!itC->second->checkModes(SECRET) || itC->second->isMember(msg._client)) {
					msg._client->reply(RPL_NAMREPLY(_server->getHostname(), msg._client->getNickname(), itC->first,
													itC->second->getMemberList()));
					msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), itC->first));
				}
			}
		}
	}
}