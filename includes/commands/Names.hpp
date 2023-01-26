#ifndef NAMES_HPP
#define NAMES_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class Names : public Command
{
    public:
		/* QoL typedefs */
		typedef std::map<std::string, Channel *> ChannelList;

        /* Constructors & Destructor */
        Names(Server *server);
        ~Names();

        /* Public Member Functions */
        bool                validate(const Message& msg);
        void                execute(const Message& msg);

    private:
		std::string 		_target;
		bool 				_hasTarget;

};

#endif