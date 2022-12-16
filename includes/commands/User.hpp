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
        User(Server* server);
        ~User();

        /* Public Member Functions */
        bool		validate(const Message& msg);
        void		execute(const Message& msg);

    private:

};

#endif