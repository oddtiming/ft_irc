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

	// FIXME : lower case channel names 
	std::string valideChanName = "#&";
	std::vector<std::string> channels;
	std::vector<std::string> keys; 
	std::map<std::string, std::string> m;

	if (msg.getMiddle().size() < 1)
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
	std::string chan = msg.getMiddle().at(0);

	size_t pos = chan.find(',');
	/*if (chan.at(0) == '0')
		LEAVEALLCHANNEL;*/
	while (pos = chan.find(',') != std::string::npos)
	{
		if(chan.at(0) != '#')
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
	for (; it != m.end(); it++)
	{
		/*if (TOOMANYCHANNELREADY){
			msg._client->reply(ERR_TOOMANYCHANNELS(it->first));
			return false;
		}
		else */if (!_server->doesChannelExist(it->first))
			_server->createChannel(it->first, it->second, msg._client);
		else if (_server->getChannelPtr(it->first)->checkMemberModes(msg._client, 'i') && _server->getChannelPtr(it->first)->checkModes('i')) {
			msg._client->reply(ERR_INVITEONLYCHAN(it->first));
			return false;
		}
		else if (!_server->channelCheckPass(it->first, it->second)) {
			msg._client->reply(ERR_BADCHANNELKEY(it->first));
			return false;
		}
		/*else if(CHANNELISFULL){
			msg._client->reply(ERR_CHANNELISFULL(it->first));
			return false;
		}*/
		else if (_server->getChannelPtr(it->first)->checkMemberModes(msg._client, 'b')){
			msg._client->reply(ERR_BANNEDFROMCHAN(it->first));
			return false;
		}
		else if (it->first.size() > 50){
			msg._client->reply(ERR_BADCHANMASK(it->first));
			return false;
		}
		else{
			_server->getChannelPtr(it->first)->addMember(_server->getClientPtr(it->first), 0);
			//SENDJOINMESSAGE;
			if (_server->getChannelPtr(it->first)->getTopic().size() > 0)
				msg._client->reply(RPL_TOPIC(it->first, _server->getChannelPtr(it->first)->getTopic()));
			else
				msg._client->reply(RPL_NOTOPIC(it->first));
		}
	}
}

void	Join::execute(const Message& msg) {
	if (validate(msg))
	{
		/* Check if channel exists */
		
		/* If channel doesn't exist, set create and set user as admin */


	}
}

