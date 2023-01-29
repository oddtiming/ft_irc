
#pragma once

/* Local Includes */
#include "Bot.hpp"

class DictBot : public Bot
{
  public:
	DictBot(
	  string name, string servName, string channel, int port, string pass = string( ));
	~DictBot( );

	/* Base Abstract Class overloads */
	virtual int reply(std::string command);
	int         _parse_definitions(const std::string& html);
};
