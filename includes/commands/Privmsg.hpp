#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class Privmsg : public Command
{
    public:
        /* Constructors & Destructor */
        Privmsg(Server* server);
        ~Privmsg();

        /* Public Member Functions */
        bool				validate(const Message& msg);
        void				execute(const Message& msg);

    private:
        std::string _target;
        bool        _targetIsChannel;

};

#endif