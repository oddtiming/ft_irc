#ifndef QUIT_HPP
#define QUIT_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Quit : public Command
{
	public:
		typedef std::map<std::string, Channel *> ChannelList;
	/* Constructors & Destructor */
		Quit(Server* server);
		~Quit() { };

		/* Public Member Functions */
		bool                validate(const Message& msg);
		void                execute(const Message& msg);
};

#endif