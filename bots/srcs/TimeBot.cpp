/* Local includes */
#include "TimeBot.hpp"

TimeBot::TimeBot(string name, string servName, string channel, int port, string pass)
  : Bot(name, servName, channel, port, pass) {}

TimeBot::~TimeBot( ) {}

int TimeBot::reply(std::string sentence) {
	(void)sentence;
	time_t now = time(0);
	_reply     = ctime(&now);
	_reply.insert(0, "The current time is ");
	if (sendChannelMessage( ))
		return 1;
	return 0;
}
