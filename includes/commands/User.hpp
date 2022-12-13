#ifndef USER_HPP
# define USER_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class User : public Command
{
    public:
        /* Constructors & Destructor */
        User();
        ~User();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif