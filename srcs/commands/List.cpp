#include "../../includes/commands/List.hpp"

List::List(Server* server) : Command("list", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

List::~List() {

}

bool	List::validate(const Message& msg) {
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

void	List::execute(const Message& msg) {

	if (validate(msg)) {
		if (!_hasTarget) {
			/*if the command has no _target, iterate over every channel and sends back its name and topic as reply*/
			std::map<std::string, Channel *> channelList = _server->getChannelList();
			std::map<std::string, Channel *>::iterator it = channelList.begin();
			for (; it != channelList.end(); ++it) {
				if (!it->second->checkModes(SECRET) || it->second->isMember(msg._client))
					msg._client->reply(RPL_LIST(it->first, std::to_string(it->second->getMemberVector().size()),
												it->second->getTopic()));
			}
		}
		else
			/*if the command has a _target, sends back its name and topic as reply*/
			msg._client->reply(RPL_LIST(_server->getChannelPtr(_target)->getName(),
										std::to_string(_server->getChannelPtr(_target)->getMemberVector().size()),
										_server->getChannelPtr(_target)->getTopic()));
		msg._client->reply(RPL_LISTEND(_server->getHostname(), msg._client->getNickname()));

	}
}