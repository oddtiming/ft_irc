#include "../../includes/commands/Privmsg.hpp"

/* Constructors & Destructor */
Privmsg::Privmsg(Server* server) : Command("privmsg", server) {
}

Privmsg::~Privmsg() {
}

/* Public Member Functions */

bool Privmsg::validate(const Message& msg) {
    std::vector<std::string>  args = msg.getMiddle();

	/* Ensure there's a target for the message */
    if (args.size() < 1)
    {
        msg._client->reply(ERR_NORECIPIENT(msg.getCommand()));
        return false;
    }
    if (_message.empty())
    {
        msg._client->reply(ERR_NOTEXTTOSEND());
        return false;
    }
    _target = args.at(0);
    args.erase(args.begin());

    _targetIsChannel = false;
    if (_target.at(0) == '#')
    {
        _targetIsChannel = true;
        if(!_server->doesChannelExist(_target))
        {
			msg._client->reply(ERR_NOSUCHCHANNEL(_target));
			return false;
        }
		if (!_server->getChannelPtr(_target)->isMember(msg._client))
		{
			 msg._client->reply(ERR_CANNOTSENDTOCHAN(_target));
			 return false;
		}
        return true;
    }

    if (!_server->doesNickExist(_target))
    {
        msg._client->reply(ERR_NOSUCHNICK(_target));
        return false;
    }
    if (_server->getClientPtr(_target)->checkGlobalModes(AWAY))
    {
        msg._client->reply(RPL_AWAY(_server->getHostname(), msg._client->getNickname(), _target, _server->getClientPtr(_target)->getAwayMessage()));  //return(_nickname + " :" + _awayMessage)
        return false;
    }
    return true;
}

void	Privmsg::execute(const Message& msg)
{
    _buildMessage(msg);

	if (!validate(msg))
        return ;

    if (_targetIsChannel)
        _server->getChannelPtr(_target)->sendToOthers(
            CMD_PRIVMSG(_buildPrefix(msg), _target, _message), msg._client);
    else
        _server->getClientPtr(_target)->reply(
            CMD_PRIVMSG(_buildPrefix(msg), _target, _message));
}

void    Privmsg::_buildMessage(const Message& msg)
{
    /* Clear the message buffer, since the Privmsg object never gets out of scope */
    _message.clear();
    
	size_t	nb_args = msg.getMiddle().size();
	
	/* If the message was a single word, some clients (e.g. Limechat) do not
		prepend a ':' before it, so it stays in the msg's _middle field */
	for (size_t i = 2; i < nb_args; ++i)
        _message.append(msg.getMiddle().at(i));
    _message.append(msg.getTrailing());
}
