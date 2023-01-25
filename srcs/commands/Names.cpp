#include "../../includes/commands/Names.hpp"

Names::Names(Server* server) : Command("names", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Names::~Names() {

}
/*
 * issues: /names #new,#new1 duplicates first channel
 * 			/names #new,#new2, segfaults
 * 			/names #new, #new1 segfaults
 * */
bool	Names::validate(const Message& msg) {
	if (!msg.hasMiddle() || msg.getMiddle().at(0).size() < 1) {
		_hasTarget = false;
	}
	else {
		_hasTarget = true;
		_target = msg.getMiddle().at(0);
		size_t pos;
		while ((pos = _target.find(',')) != std::string::npos)
		{
			std::cerr << "_target.substr(0,pos) =, size " << _target.substr(0,pos) << " " << _target.substr(0,pos).size() << std::endl;
			_targetList.push_back(_target.substr(0,pos));
			_target.erase(0, pos + 1);
		}
		_targetList.push_back(_target);
		std::vector<std::string>::iterator it = _targetList.begin();
		for (; it != _targetList.end(); ++it)
			std::cout << "_target list has " << *it << std::endl;
		it = _targetList.begin();
		for (; it != _targetList.end(); ++it)
		{
			std::cerr << "check if chan exists " << *it << std::endl;
			if (!_server->doesChannelExist(*it)) {
				std::cerr << "bye" << std::endl;
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
		std::cerr << "is here " << "with _hasTarget " << _hasTarget << std::endl;
		if (_hasTarget)
		{
			std::vector<std::string>::iterator it = _targetList.begin();
			std::vector<std::string>::iterator ite = _targetList.end();
			for (; it != ite; ++it)
			{
				std::cerr << "target is " << *it << std::endl;
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