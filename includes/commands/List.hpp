#ifndef LIST_HPP
#define LIST_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class List : public Command
{
    public:
        /* Constructors & Destructor */
        List();
        ~List();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif