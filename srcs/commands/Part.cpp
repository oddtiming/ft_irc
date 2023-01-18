#include "commands/Part.hpp"

/* Constructors & Destructor */
Part::Part() : Command("Part") {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Part::~Part() {
}

/* Public Member Functions */
bool	Part::validate(const Message& msg) {
    if (msg.getMiddle().size() < 1)
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));

    std::string valideChanName = "#&!+";
	std::string chan = msg.getMiddle().at(0);
    std::string leaveMsg = msg.getTrailing();

    std::vector<std::string> channels;
	size_t pos;

	while (pos = chan.find(',') != std::string::npos)
	{
		channels.push_back(chan.substr(0, pos));
		chan.erase(0, pos + 1);
	}
    if (chan.size() > 0)
		channels.push_back(chan);
    std::vector<std::string>::iterator it = channels.begin();
	for (; it != channels.end(); it++){
        if (!_server->isUserChannelMember(msg._client->getNickname()))
            msg._client->reply(ERR_NOTONCHANNEL(*it));
        else if(!valideChanName.find(chan.at(0)))
			msg._client->reply(ERR_NOSUCHCHANNEL(*it));
        else
        {
            //TODO: add code to remove user from channel
            if (leaveMsg.size() > 0)
                msg._client->reply("User" + msg._client->getNickname() + " leaving channel" + *it + " with the message \"" + leaveMsg + "\"");
            else
                msg._client->reply("User" + msg._client->getNickname() + " leaving channel" + *it);
        }
    }
}


void	Part::execute(const Message& msg) {

    if (validate(msg))
        {
            /* Check permissions for execution of function */

        }
    
}