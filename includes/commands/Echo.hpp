#ifndef ECHO_HPP
#define ECHO_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Echo : public Command
{
    public:
        /* Constructors & Destructor */
        Echo();
        ~Echo();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif