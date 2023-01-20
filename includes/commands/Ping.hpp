#ifndef PING_HPP
#define PING_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Ping : public Command
{
    public:
        /* Constructors & Destructor */
        Ping(Server* server);
        ~Ping();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif