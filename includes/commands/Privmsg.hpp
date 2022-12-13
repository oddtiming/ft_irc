#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Privmsg : public Command
{
    public:
        /* Constructors & Destructor */
        Privmsg();
        ~Privmsg();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif