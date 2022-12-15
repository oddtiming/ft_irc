#include "commands/User.hpp"
#include "Server.hpp"

/* Constructors & Destructor */
User::User() : Command("user") {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

User::~User() {

}

/* Public Member Functions */

bool		User::validate(const Message& msg) {

    if (msg.getMiddle().size() < 1) {
        //buildReply(ERR_NEEDMOREPARAMS(msg.getCommand()));
        return false;
    }
    if (msg._client->getUsername().size() != 0) {
        //buildReply(ERR_ALREADYREGISTRED());
        return false;
    }
}

void		User::execute(const Message& msg) {
    if (validate(msg))  {
        msg._client->setUsername(msg.getMiddle().at(0));
        /*if (msg.getMiddle().size() > 1 && msg.getMiddle().at(1).isdigit())
            msg._client->modifyGlobalModes(stoi(msg.getMiddle().at(1)), 0); // FIXME: call mode cmd */
       /* if (msg.getTrailing().size() > 0)
            msg._client->setRealName(msg.getTrailing()); */
        // FIXME: add realName to _client
    }
}
