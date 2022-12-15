#ifndef INFO_HPP
#define INFO_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Info : public Command
{
    public:
        /* Constructors & Destructor */
        Info();
        ~Info();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:

};

#endif