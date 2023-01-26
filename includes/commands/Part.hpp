#ifndef PART_HPP
#define PART_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Part : public Command
{
    public:
        /* Constructors & Destructor */
        Part(Server* server);
        ~Part();

        /* Public Member Functions */
        bool                parse(const Message& msg);
        bool                validate(const std::string& channel, Client *client);
        void                execute(const Message& msg);

    private:
        Client*                     _client;
        std::vector<std::string>    _targets;
        std::string                 _partMsg;
        Channel *                   _currTarget;

        /* FIXME: not needed for now, but it was there when I overhauled the fct, ask @alx if he uses it */
        static std::string          _validChanPrefixes;
};



#endif