#ifndef EXIT_HPP
#define EXIT_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Exit : public Command
{
    public:
        /* Constructors & Destructor */
        Exit();
        ~Exit();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif