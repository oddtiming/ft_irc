#include "commands/Nick.hpp"
#include "Server.hpp"

Nick::Nick(Server* server) : Command("nick", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();
	std::string					nick;

	if (middle.size() == 0) {
		// msg.getReplies(ERR_NONICKNAMEGIVEN);
		// msg._client->reply(this->_buildReply(ERR_NONICKNAMEGIVEN));
		std::cerr << "ERR_NONICKNAMEGIVEN" << std::endl;
		return false;
	}

	nick = middle.at(0);

	if (nick.size() > 9) {
		// msg.getReplies(ERR_ERRONEUSNICKNAME);
		std::cerr << "ERR_ERRONEUSNICKNAME" << std::endl;
		return false;
	}
	
	if (_server->doesNickExist(nick)) {
		// msg.getReplies(ERR_NICKNAMEINUSE);
		std::cerr << "ERR_NICKNAMEINUSE" << std::endl;
		return false;
	}
	return true;
}

void Nick::execute(const Message &msg) {
	std::string	nick = msg.getMiddle().at(0);

	if (validate(msg)) {
		msg._client->setNickname(nick);
		// msg._client->reply(_buildPrefix(msg) + " NICK :" + nick + "\n");
		// NOTE: if valid, NICK needs no reply
	}
	//send new nick msg to all relevant user, format:
	//(msg._user->getNickname() + "!" + msg._user->getUsername()
	// + "@" + _server->getName()+ " NICK :" + name + "\n").c_str();
}
