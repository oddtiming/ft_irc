#ifndef NOTICE_HPP
#define NOTICE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Notice : public Command {
public:
	/* Constructors & Destructor */
	Notice(Server* server);
	~Notice() { }

	/* Public Member Functions */
	bool	validate(const Message& msg);
	void    execute(const Message& msg);

private:

};

#endif
