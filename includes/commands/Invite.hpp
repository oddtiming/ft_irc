#ifndef INVITE_HPP
#define INVITE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Invite : public Command {
public:
	/* Constructors & Destructor */
	Invite(Server* server);
	~Invite() { }

	/* Public Member Functions */
	bool	validate(const Message& msg);
	void    execute(const Message& msg);

private:

};

#endif
