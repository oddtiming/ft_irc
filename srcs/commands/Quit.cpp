#include "commands/Quit.hpp"

Quit::Quit(Server* server) : Command("quit", server) { }

bool	Quit::validate(const Message& msg) {

	std::string tmp = msg.getCommand();
	return (true);
}

void	Quit::execute(const Message& msg) {
	/* Check permissions for execution of function */
	std::string message = "";
	if (!msg.getTrailing().empty())
	{
		message += msg.getTrailing();
	}
	Client *_client = msg._client;
	/* Iterate over every channel to see which the target user is part of*/
	ChannelList channelList = _server->getChannelList();
	ChannelList::iterator it = channelList.begin();
	ChannelList::iterator ite = channelList.end();
	for (; it != ite; ++it)
	{
		/*Sends a quit message to every user on the channel then remove client member*/
		if (it->second->isMember(_client))
			it->second->sendToAll(CMD_PART(_buildPrefix(msg), it->first, message));
	}
	_server->removeClient(msg._client);
}