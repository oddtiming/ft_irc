#include "commands/Privmsg.hpp"

/* Constructors & Destructor */
Privmsg::Privmsg() : command("privmsg") {    
}

Privmsg::~Privmsg() {
}

/* Public Member Functions */

bool Privmsg::validate(const Message& msg) {
    std::vector<std::string>    args = msg.getParams();

    if(args.size() == 0)
    {
        msg.getReplies(ERR_NORECIPIENT);
        return false;
    }
    if (args.size() == 1)
    {
        msg.getReplies(ERR_NOTEXTTOSEND());
        return false;
    }
    _target = args[0];
    args.erase(args.begin());
    if(_target[0] == '#')
    {
        _targetIsChannel = true;
        if(!(_server->doesChannelExist(_target)) ||
         !(_server->isUserChannelMember(msg._client.getNickname()) || !(checkChannelModes(_target, BAN | INV_ONLY))))
        {
            msg.getReplies(ERR_CANNOTSENDTOCHAN(_target));
            return false;
        }
    }
    else
    {
        _targetIsChannel = false;
        if (msg._client->checkGlobalMode(AWAY))
        {
            msg.getReplies(RPL_AWAY(_target, _server->getClient(target)->_awayMessage));  //return(_nickname + " :" + _awayMessage) 
            return false;
        }
        if (!(_server.doesNickExist(_target)))
        {
            msg.getReplies(ERR_NOSUCHNICK(msg._client.getNickname()));
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

