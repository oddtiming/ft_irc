#include "commands/Privmsg.hpp"

/* Constructors & Destructor */
Privmsg::Privmsg() : Command("privmsg") {    
}

Privmsg::~Privmsg() {
}

/* Public Member Functions */

bool Privmsg::validate(const Message& msg) {
    std::vector<std::string>  args = msg.getMiddle();

    if(args.size() == 0)
    {
        buildReply(ERR_NORECIPIENT(msg.getCommand()));
        return false;
    }
    if (args.size() == 1)
    {
        buildReply(ERR_NOTEXTTOSEND());
        return false;
    }
    _target = args.at(0);
    args.erase(args.begin());
    if(_target.at(0) == '#')
    {
        _targetIsChannel = true;
        if(!(_server->doesChannelExist(_target)) ||
            !(_server->isUserChannelMember(msg._client->getNickname()) ||
            !(msg._client->checkChannelModes(_target, BAN | INV_ONLY))))
        {
            buildReply(ERR_CANNOTSENDTOCHAN(_target));
            return false;
        }
    }
    else
    {
        _targetIsChannel = false;
        if (msg._client->checkGlobalMode(AWAY))
        {
            buildReply(RPL_AWAY(_target, _server->getClient(target)->_awayMessage));  //return(_nickname + " :" + _awayMessage) 
            return false;
        }
        if (!(_server.doesNickExist(_target)))
        {
            buildReply(ERR_NOSUCHNICK(msg._client.getNickname()));
            return false;
        }
    }
    return true;
}

void	Privmsg::execute(const Message& msg) {
	if (validate(msg)) {
		if (_targetIsChannel)
            //FIXME: sendto function to be added in order to message user/channel
        else
            //send to target
	}
}
/*
 *  ERR_NORECIPIENT                 
 *  ERR_NOTEXTTOSEND    
 *  ERR_CANNOTSENDTOCHAN            
 *  ERR_TOOMANYTARGETS
 *  ERR_NOSUCHNICK
 *  RPL_AWAY
 */