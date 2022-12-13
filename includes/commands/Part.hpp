#ifndef PART_HPP
#define PART_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Part : public Command
{
    public:
        /* Constructors & Destructor */
        Part();
        ~Part();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif