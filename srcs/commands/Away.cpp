#include "../../includes/commands/Away.hpp"

Away::Away(Server* server) : Command("away", server) {

}

Away::~Away() {

}

bool	Away::validate(const Message& msg) {
	if (!msg.getTrailing().empty())
		return true;
	return true;

}

void	Away::execute(const Message& msg) {
	if (validate(msg)) {
		if (msg._client->checkGlobalModes(AWAY)){
			if (msg.getTrailing().empty())
			{
				msg._client->reply(RPL_UNAWAY());
				msg._client->setGlobalModes(AWAY, true);
				msg._client->setAwayMessage("");
			}
			else
				msg._client->setAwayMessage(msg.getTrailing());
		}
		else if (!msg.getTrailing().empty())
		{
			msg._client->reply(RPL_NOWAWAY());
			msg._client->setGlobalModes(AWAY, false);
			msg._client->setAwayMessage(msg.getTrailing());
		}
	}
}
