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

bool    User::validate(const Message& msg) {
    Client* client = msg._client;
    /* If no username was entered reply error */
    if (msg.getMiddle().size() < 1) 
    {
        client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
        std::cerr << ERR_NEEDMOREPARAMS(msg.getCommand()) << std::endl;
        return false;
    }
    /* If username has already registered, do not allow re-use of USER command */
    else if (client->getUsername().size() != 0)
    {
        client->reply(ERR_ALREADYREGISTRED());
        std::cerr << ERR_ALREADYREGISTRED() << std::endl;
        return false;
    }
    //FIXME: need to handle possible user modes being passed in USER command
    /*
    Example:

    USER guest 0 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

    USER guest 8 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan", and asking to be set
                                   invisible.
    */
    return true;
}

void    User::execute(const Message& msg) {
    Client* client = msg._client;
    
    /* Attempt to validate username*/
    if (validate(msg))
        client->setUsername(msg.getMiddle().at(0));

    /* If username and nickname have been successfully added, register user*/
    if (!client->getUsername().empty() && !client->getNickname().empty() && !client->getRegistration())
    {
        client->setRegistration(true);
        client->reply(RPL_WELCOME(msg._client->getNickname(), _buildPrefix(msg)));
        if (DEBUG)
            std::cout << getTimestamp() <<  GREEN "New user successfully registered: " CLEAR << client->getNickname() << std::endl << std::endl;
    }
    /*if (msg.getMiddle().size() > 1 && msg.getMiddle().at(1).isdigit()) // FIXME: make sure only +i / +w can be passed
        msg._client->modifyGlobalModes(stoi(msg.getMiddle().at(1)), 0); // FIXME: call mode cmd */
    /* if (msg.getTrailing().size() > 0)
        msg._client->setRealName(msg.getTrailing()); */
    // FIXME: add realName to _client
}
