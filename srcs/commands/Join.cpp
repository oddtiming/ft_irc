#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join(Server* server) : Command("join", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */
bool	Join::validate(const Message& msg) {

	std::string valideChanName = "#&!+";
	std::vector<std::string> channels;
	std::vector<std::string> keys; 
	std::map<std::string, std::string> m;
	if (msg.getMiddle().size() < 1)
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
	std::string chan = msg.getMiddle().at(0);

	size_t pos = chan.find(',');

	while (pos = chan.find(',') != std::string::npos)
	{
		if(!valideChanName.find(chan.at(0)))
		{
			msg._client->reply(ERR_BADCHANMASK(chan.substr(0, pos)));
			chan.erase(0, pos + 1);
			continue;
		}
		channels.push_back(chan.substr(0, pos));
		chan.erase(0, pos + 1);
	} 
	if (chan.size() > 0)
		channels.push_back(chan);

	if (msg.getMiddle().size() > 1) {
		std::string k = msg.getMiddle().at(1);
		while (pos = k.find(',') != std::string::npos)
		{
			keys.push_back(k.substr(0, pos));
			k.erase(0, pos + 1);
		}
		if (k.size() > 0)
			keys.push_back(k);
	}
	for (int i = 0; i < channels.size(); i++) {
		if (i < keys.size())
			m[channels.at(i)] = keys.at(i);
		else
			m[channels.at(i)] = "";
	}
	std::map<std::string, std::string>::iterator it = m.begin();
	for (; it++; it != m.end())
	{
		if (!_server->doesChannelExist(it->first))
			_server->createChannel(it->first, it->second, msg._client);
		else if (_server.checkModes('i', it->first)) {
			msg._client->reply(ERR_INVITEONLYCHAN(it->first));
			return false;
		}
		else if (BADPASSWORD) {
			msg._client->reply(ERR_BADCHANNELKEY(it->first));
			return false;
		}
		else if (BANNEDFROMCHAN){
			msg._client->reply(ERR_BANNEDFROMCHAN(it->first));
			return false;
		}
		else
			_server->channelAddMember(it->first, it->second);
	}
}

void	Join::execute(const Message& msg) {
	if (validate(msg))
	{
		/* Check if channel exists */
		
		/* If channel doesn't exist, set create and set user as admin */


	}
}

