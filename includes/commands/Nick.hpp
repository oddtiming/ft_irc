#ifndef NICK_HPP
#define NICK_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Nick : public Command {
    public:
        /* Constructors & Destructor */
        Nick(Server* server);
        ~Nick() { }

        /* Public Member Functions */
		bool	validate(const Message& msg);
        void    execute(const Message& msg);

    private:
        std::string _nick;
        Client*		_client;

};

#endif
