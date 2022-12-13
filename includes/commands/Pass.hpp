#ifndef PASS_HPP
#define PASS_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Pass : public Command
{
    public:
        /* Constructors & Destructor */
        Pass();
        ~Pass();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif