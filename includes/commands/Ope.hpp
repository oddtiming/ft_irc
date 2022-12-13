#ifndef OPE_HPP
#define OPE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Ope : public Command
{
    public:
        /* Constructors & Destructor */
        Ope();
        ~Ope();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif