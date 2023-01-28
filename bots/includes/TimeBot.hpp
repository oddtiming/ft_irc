
#pragma once

/* Local Includes */
#include "Bot.hpp"

class TimeBot : public Bot
{
  public:
	TimeBot(
	  string name, string servName, string channel, int port, string pass = string( ));
	~TimeBot( );

	/* Base Abstract Class overloads */
	virtual int reply(std::string command);
};
