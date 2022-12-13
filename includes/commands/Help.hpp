#ifndef HELP_HPP
#define HELP_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Help : public Command
{
    public:
        /* Constructors & Destructor */
        Help();
        ~Help();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif