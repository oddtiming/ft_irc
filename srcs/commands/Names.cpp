#include "../../includes/commands/Names.hpp"

Names::Names(Server* server) : Command("names", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Names::~Names() {

}

bool	Names::validate(const Message& msg) {
	if (msg.getMiddle().size() < 2) {
		_hasTarget = false;
	}
	else {
		_hasTarget = true;
		_target = msg.getMiddle().at(0);
		std::string::difference_type n = std::count(_target.begin(), _target.end(), ',');

		if (n > 0)
		{
			size_t pos;
			while ((pos = _target.find(',')) != std::string::npos)
			{
				_targetList.push_back(_target.substr(0,pos));
				_target.erase(0, pos + 1);
			}
		}
		_targetList.push_back(_target);
		std::vector<std::string>::iterator it = _targetList.begin();
		for (; it != _targetList.end(); ++it)
		{
			if (!_server->doesChannelExist(*it)) {
				msg._client->reply(ERR_NOSUCHCHANNEL(_server->getHostname(), msg._client->getNickname(), *it));
				_targetList.erase(it);
				msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), *it));

			}
		}
		if (_targetList.empty())
			return false;
	}

	return true;
}

void	Names::execute(const Message& msg) {

	if (validate(msg)) {
		if (_hasTarget)
		{
			std::vector<std::string>::iterator it = _targetList.begin();
			std::vector<std::string>::iterator ite = _targetList.end();
			for (; it != ite; ++it)
			{
				msg._client->reply(RPL_NAMREPLY(_server->getHostname(), msg._client->getNickname(), *it, _server->getChannelPtr(*it)->getMemberList()));
				msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), *it));
			}

		}
		else
		{
			ChannelList channelList = _server->getChannelList();
			ChannelList::iterator itC = channelList.begin();
			ChannelList::iterator iteC = channelList.end();
			for (; itC != iteC; ++itC)
			{
				msg._client->reply(RPL_NAMREPLY(_server->getHostname(), msg._client->getNickname(), itC->first, itC->second->getMemberList()));
				msg._client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), itC->first));
			}
		}
	}
}