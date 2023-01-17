#include "../../includes/commands/Privmsg.hpp"

/* Constructors & Destructor */
Privmsg::Privmsg(Server* server) : Command("privmsg", server) {
}

Privmsg::~Privmsg() {
}

/* Public Member Functions */

bool Privmsg::validate(const Message& msg) {
    std::vector<std::string>  args = msg.getMiddle();

	/*make sure there's a target for the message*/
    if(args.size() == 0)
    {
        msg._client->reply(ERR_NORECIPIENT(msg.getCommand()));
        return false;
    }
	/*and a message to send*/
    if (args.size() == 1)
    {
		msg._client->reply(ERR_NOTEXTTOSEND());
        return false;
    }
    _target = args.at(0);
    args.erase(args.begin());
    if(_target.at(0) == '#')
    {
        _targetIsChannel = true;
        if(!_server->doesChannelExist(_target))
			return false;
         if (!_server->getChannelPtr(_target)->isMember(msg._client))
		 {
			 msg._client->reply(ERR_CANNOTSENDTOCHAN(_target));
			 return false;
		 }         if (!_server->getChannelPtr(_target)->checkMemberModes(msg._client, BAN | INV_ONLY))
        {
			msg._client->reply(ERR_CANNOTSENDTOCHAN(_target));
            return false;
        }
    }
    else
    {
        _targetIsChannel = false;
        if (_server->getClientPtr(_target)->checkGlobalModes(AWAY))
        {
			msg._client->reply(RPL_AWAY(_target, _server->getClientPtr(_target)->getAwayMessage()));  //return(_nickname + " :" + _awayMessage)
            return false;
        }
        if (!(_server->doesNickExist(_target)))
        {
			msg._client->reply(ERR_NOSUCHNICK(_target));
            return false;
        }
    }
    return true;
}

void	Privmsg::execute(const Message& msg) {
	if (validate(msg)) {
		if (_targetIsChannel) {
			std::vector<std::string> target = _server->getChannelPtr(_target)->getMemberVector();
			std::vector<std::string>::iterator it = target.begin();
			for (; it != target.end(); ++it)
				_server->getClientPtr(*it)->reply(msg.getMiddle().at(1));
		}
        else
			_server->getClientPtr(_target)->reply(msg.getMiddle().at(1));
		//fixme: add channel messaging
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