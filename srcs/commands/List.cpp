#include "../../includes/commands/List.hpp"

List::List(Server* server) : Command("list", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

List::~List() {

}
//#define RPL_LIST(hostname,nick,channel,nbUsers,topic) ":" + (hostname) + " 323 " + (nick) + " " + (channel) + " " + (nbUsers) + " : " + (topic) +  "\r\n" //322
//#define RPL_LISTEND(host, nick) ":" + (host) + " 323 " + (nick) + " :End of channel list.\r\n" //323

bool	List::validate(const Message& msg) {
	if (msg.getCommand() == msg.getCommand())
		return true;

	return true;
}

void	List::execute(const Message& msg) {

	if (validate(msg))
	{
		std::map<std::string, Channel *> channelList = _server->getChannelList();
		//fixme: figure out a way to access the list of channel, most likely a getChannelList in channel.hpp
		std::map<std::string, Channel *>::iterator it = channelList.begin();
		for (; it != channelList.end(); ++it)
		{
			if (!it->second->checkModes(SECRET) || it->second->isMember(msg._client))
				msg._client->reply(RPL_LIST(it->first, std::to_string(it->second->getMemberVector().size()), it->second->getTopic()));
		}
		msg._client->reply(RPL_LISTEND(_server->getHostname(), msg._client->getNickname()));

	}
}