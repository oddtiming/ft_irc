#ifndef LIST_HPP
#define LIST_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class List : public Command
{
public:
	/* Constructors & Destructor */
	List(Server *server);
	~List();

	/* Public Member Functions */
	bool                validate(const Message& msg);
	void                execute(const Message& msg);

private:
	bool 				_hasTarget;
	std::string 		_target;

};

#endif