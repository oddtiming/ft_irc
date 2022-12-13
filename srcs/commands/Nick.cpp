#include "commands/Nick.hpp"
#include "Server.hpp"

Nick::Nick() : Command("nick") {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string> nick = msg.getParams();
	if (nick.size() == 0) {
		// msg.getReplies(ERR_NONICKNAMEGIVEN);
		msg._client->reply(this->_buildReply(ERR_NONICKNAMEGIVEN));
		return false;
	}
	else if (nick.at(0).size() > 9) {
		msg.getReplies(ERR_ERRONEUSNICKNAME);
		return false;
	}
	if (_server->doesNickExist(nick.at(0))) {
		msg.getReplies(ERR_NICKNAMEINUSE);
		return false;
	}
	return (true);
}

void Nick::execute(const Message &msg) {
	std::string	nick = msg.getParams()[0];

	if (validate(msg)) {
		msg._client->setNickname(nick);
		msg._client->reply(_buildPrefix(msg) + " NICK :" + nick + "\n");
	}

	//send new nick msg to all relevant user, format:
	//(msg._user->getNickname() + "!" + msg._user->getUsername()
	// + "@" + _server->getName()+ " NICK :" + name + "\n").c_str();
}