#ifndef PART_HPP
#define PART_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Part : public Command {
    public:
        /* Constructors & Destructor */
        Part(Server* server);
        ~Part() { }

        /* Public Member Functions */
        bool                parse(const Message& msg);
        bool                validate(const std::string& channel, Client *client);
        void                execute(const Message& msg);
        void                clearData();

    private:
        Client*                     _client;
        Channel *                   _currTarget;
        std::vector<std::string>    _targetChannels;
        std::string                 _message;
};



#endif