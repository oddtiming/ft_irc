#ifndef NAMES_HPP
#define NAMES_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Names : public Command
{
    public:
        /* Constructors & Destructor */
        Names();
        ~Names();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif