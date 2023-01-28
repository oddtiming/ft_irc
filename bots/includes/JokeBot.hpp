
#pragma once

/* Local Includes */
#include "Bot.hpp"

class JokeBot : public Bot
{
  public:
	JokeBot(
	  string name, string servName, string channel, int port, string pass = string( ));
	~JokeBot( );

	/* Base Abstract Class overloads */
	virtual int reply(std::string command);

  private:
	void _parse_joke(std::string html);
	void _parse_joke_html_field(std::string raw, std::string start, std::string end);
};
