#include "commands/Nick.hpp"
#include "Server.hpp"

Nick::Nick(Server* server) : Command("nick", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

bool Nick::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();
	Client*						client = msg._client;	

	/* Check if a nickname was given */
	if (middle.empty())
	{
		client->reply(ERR_NONICKNAMEGIVEN());
		std::cerr << "ERR_NONICKNAMEGIVEN" << std::endl;
		return false;
	}
	std::string	nick = middle.at(0);
	/* If nickname is too long, return error */
	if (nick.size() > 9)
	{
		client->reply(ERR_ERRONEUSNICKNAME(nick));
		return false;
	}
	/* If nickname is already in use, return error */
	if (_server->doesNickExist(nick))
	{
		//FIXME: Should be using this reply when nickname is in use upon connection, ERR_NICKNAMEINUSE is used when existing user attempts to change nickname
		//NICK can never fail on connection, the server will simply provide a new nickname (Nick + _)
		/*
		ERR_NICKCOLLISION
		<nick> :Nickname collision KILL from <user>@<host>
		*/
		client->reply(ERR_NICKNAMEINUSE(nick));
		std::cerr << "ERR_NICKNAMEINUSE" << std::endl;
		return false;
	}
	return true;
}

void Nick::execute(const Message &msg) {
	
	//FIXME: move data to Nick class to remove redundant actions
	if (validate(msg)) {
		std::string	nick = msg.getMiddle().at(0);
		msg._client->setNickname(nick);
	}

}

/*                        

Response for changing a nickname
:WiZ!jto@tolsun.oulu.fi NICK Kilroy
                           ; Server telling that WiZ changed his
                           nickname to Kilroy.

*/