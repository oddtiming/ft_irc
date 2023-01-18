#include "../../includes/commands/Privmsg.hpp"

/* Constructors & Destructor */
Privmsg::Privmsg(Server* server) : Command("privmsg", server) {
}

Privmsg::~Privmsg() {
}

/* Public Member Functions */

bool Privmsg::validate(const Message& msg) {
    std::vector<std::string>  args = msg.getMiddle();


    /* DEBUG */
    std::cout << "Prefix" << std::endl;
    std::cout << YELLOW << msg.getPrefix() << CLEAR << std::endl;
    std::cout << "Middle" << std::endl;

    std::vector<std::string>::const_iterator it = msg.getMiddle().begin();
    for (; it != msg.getMiddle().end(); ++it)
        std::cout << YELLOW << *it << CLEAR << std::endl;

    std::cout << "Prefix" << std::endl;
    std::cout << YELLOW << msg.getPrefix() << CLEAR << std::endl;

    //FIXME: TRAILING CAN BE MULTIPLE WORDS

	/*make sure there's a target for the message*/
    if(args.size() == 0)
    {
        msg._client->reply(ERR_NORECIPIENT(msg.getCommand()));
        return false;
    }
	/*and a message to send*/
   /* if (args.size() == 1)
    {
		msg._client->reply(ERR_NOTEXTTOSEND());
        return false;
    }*/
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
		}
		/*if (!_server->getChannelPtr(_target)->checkMemberModes(msg._client, BAN | INV_ONLY))
        {
			msg._client->reply(ERR_CANNOTSENDTOCHAN(_target));
            return false;
        }*/
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
        //FIXME: Structure for single word and multiple word messages is different
        /*
        Single word:
            PRIVMSG #new hello
        Multiple word:
            PRIVMSG #new :hello hello
        */

		std::string message = ":"+_buildPrefix(msg) + " " + msg.getCommand() + " " + _target + " :" + msg.getTrailing();
		std::vector<std::string>::const_iterator it = msg.getMiddle().begin();
		for (; it != msg.getMiddle().end(); ++it)
			std::cout << YELLOW << *it << CLEAR << std::endl;
        /* If target of a message is a channel */
		if (_targetIsChannel) {
            std::cout << YELLOW << "this" << CLEAR << std::endl;
			_server->getChannelPtr(_target)->replyToAll(message, msg._client);


		}
        /* If target is another user */
        else
			_server->getClientPtr(_target)->reply(message);
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