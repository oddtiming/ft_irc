#include "commands/User.hpp"
#include "Server.hpp"

/* Constructors & Destructor */
User::User(Server *server) : Command("user", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

User::~User() {

}

/* Public Member Functions */

bool		User::validate(const Message& msg) {
	std::string					nick;

    /* If no username was entered reply error */
    if (msg.getMiddle().size() < 1) 
    {
        msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
        std::cerr << ERR_NEEDMOREPARAMS(msg.getCommand()) << std::endl;
        return false;
    }
    /* If username has already registered, do not allow re-use of USER command */
    else if (msg._client->getUsername().size() != 0)
    {
        msg._client->reply(ERR_ALREADYREGISTRED());
        std::cerr << ERR_ALREADYREGISTRED() << std::endl;
        return false;
    }
    return true;
}

void		User::execute(const Message& msg) {
    if (validate(msg) == false)
        return ;
    msg._client->setUsername(msg.getMiddle().at(0));
    msg._client->reply(RPL_WELCOME(msg._client->getNickname(), _buildPrefix(msg)));
    /*if (msg.getMiddle().size() > 1 && msg.getMiddle().at(1).isdigit()) // FIXME: make sure only +i / +w can be passed
        msg._client->modifyGlobalModes(stoi(msg.getMiddle().at(1)), 0); // FIXME: call mode cmd */
    /* if (msg.getTrailing().size() > 0)
        msg._client->setRealName(msg.getTrailing()); */
    // FIXME: add realName to _client
}
