#include "commands/Nick.hpp"
#include "Server.hpp"

Nick::Nick(Server* server) : Command("nick", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();
	std::string					nick = middle.at(0);

	if (middle.size() == 0)
	{
		msg._client->reply(ERR_NONICKNAMEGIVEN());
		std::cerr << "ERR_NONICKNAMEGIVEN" << std::endl;
		return false;
	}
	/* If nickname is too long, return error */
	else if (nick.size() > 9)
	{
		msg._client->reply(ERR_ERRONEUSNICKNAME(nick));
		std::cerr << "ERR_ERRONEUSNICKNAME" << std::endl;
		return false;
	}
	/* If nickname is already in use, return error */
	else if (_server->doesNickExist(nick))
	{
		msg._client->reply(ERR_NICKNAMEINUSE(nick));
		std::cerr << "ERR_NICKNAMEINUSE" << std::endl;
		return false;
	}
	return true;
}

void Nick::execute(const Message &msg) {
	std::string	nick = msg.getMiddle().at(0);

	if (validate(msg)) {
		msg._client->setNickname(nick);
	}
	//send new nick msg to all relevant user, format:
	//_buildPrefix() + " NICK :" + nick + "\n").c_str();
}
