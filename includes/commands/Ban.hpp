#ifndef BAN_HPP
#define BAN_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Ban : public Command
{
    public:
        /* Constructors & Destructor */
        Ban(Server* server);
        ~Ban();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif