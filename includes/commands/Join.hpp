#ifndef JOIN_HPP
#define JOIN_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Join : public Command
{
    public:
        /* Constructors & Destructor */
        Join();
        ~Join();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif