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
        Kick();
        ~Kick();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif