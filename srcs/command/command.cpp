#include "Command.hpp"

void	PRIVMSG(Command *command)
{
	if (len(command->getParams()) == 0)
		return getReplies(411);
}