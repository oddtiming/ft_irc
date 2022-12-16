#ifndef KICK_HPP
#define KICK_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Kick : public Command
{
    public:
        /* Constructors & Destructor */
        Kick(Server *server);
        ~Kick();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif