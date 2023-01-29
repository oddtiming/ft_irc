#ifndef WHO_HPP
#define WHO_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class Who : public Command
{
  public:
	/* Constructors & Destructor */
	Who(Server* server);
	~Who( );

	/* Public Member Functions */
	bool validate(const Message& msg);
	void execute(const Message& msg);

  private:
	/* Private member attributes */
	bool _requestOpers;
};

#endif