#include "../../includes/commands/Names.hpp"

Names::Names(Server* server) : Command("names", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Names::~Names() {

}

bool	Names::validate(const Message& msg) {
	_hasTarget = false;
	if (msg.hasMiddle())
	{
		/*checks if the command has a _target and if said _target is valid sets it, else sends back an error without a reply */
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
			/*if the command has a _target, sends back its name and users as reply*/
			msg._client->reply(RPL_NAMREPLY(_server->getHostname(), msg._client->getNickname(), _target, _server->getChannelPtr(_target)->getMemberList()));
			msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), _target));
		}
		else
		{
			/*if the command has no _target, iterate over every channel and sends back its name and users as reply*/
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