#include "commands/User.hpp"
#include "Server.hpp"

User::User(Server *server) : Command("user", server) { }

bool    User::validate(const Message& msg) {
    Client* client = msg._client;

    /* If no username was entered reply error */
    if (msg.getMiddle().size() < 1) 
    {
        client->reply(ERR_NEEDMOREPARAMS(_server->getHostname(), _client->getNickname(), msg.getCommand()));
        return false;
    }
    /* If username has already registered, do not allow re-use of USER command */
    else if (client->getUsername().size() != 0)
    {
        client->reply(ERR_ALREADYREGISTRED(_server->getHostname()));
        return false;
    }

    return true;
}

void    User::execute(const Message& msg) {
    Client* client = msg._client;
    
    /* Attempt to validate username */
    if (validate(msg))
        client->setUsername(msg.getMiddle().at(0));

    /* If username and nickname have been successfully added, register user */
    if (!client->getUsername().empty() && !client->getNickname().empty() && !client->getRegistration())
    {
        client->setRegistration(true);
        client->reply(RPL_WELCOME(_server->getHostname(), msg._client->getNickname(), _buildPrefix(msg)));
        std::cout << getTimestamp() <<  GREEN "New user successfully registered: " CLEAR << client->getNickname() << std::endl;
    }
}