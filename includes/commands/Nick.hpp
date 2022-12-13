#ifndef NICK_HPP
#define NICK_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Nick : public Command
{
    public:
        /* Constructors & Destructor */
        Nick();
        ~Nick();

        /* Public Member Functions */
		bool                validate(const Message& msg);
        const std::string&  execute(const Message& msg);

    private:

};

#endif