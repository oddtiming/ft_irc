#ifndef PONG_HPP
#define PONG_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Pong : public Command {
    public:
        /* Constructors & Destructor */
        Pong(Server* server);
        ~Pong() { }

        /* Public Member Functions */
        void                execute(const Message& msg);
};

#endif