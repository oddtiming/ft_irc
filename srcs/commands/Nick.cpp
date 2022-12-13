#include "../../includes/commands/Nick.hpp"
#include "../../includes/replies.hpp"

Nick::Nick()
{
	_name = "nick";
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string> name = msg.getParams();
	if (name.size() == 0)
	{
		msg.getReplies(ERR_NONICKNAMEGIVEN);
		return (false);
	}
	else if (name[0].size() > 9)
	{
		msg.getReplies(ERR_ERRONEUSNICKNAME);
		return (false);
	}
	if (find(_server->_users.begin(), _server->_users.end(), name)
	{
		msg.getReplies(ERR_NICKNAMEINUSE);
		return (false);
	}
	return (true);
}

void Nick::execute(const Message &msg) {
	std::string	name = msg.getParams()[0];
	//send new nick msg to all relevant user, format:
	//(msg._user->getNickname() + "!" + msg._user->getUsername()
	// + "@" + _server->getName()+ " NICK :" + name + "\n").c_str();
	msg._user->setNickname(name);
	return (msg._user->getNickname() + "!" + msg._user->getUsername()
	 + "@" + _server-> + " NICK :" + name + "\n").c_str();
}