#include "../../includes/commands/Who.hpp"

Who::Who(Server* server) : Command("who", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Who::~Who() {

}

bool	Who::validate(const Message& msg) {
	if (!msg.hasMiddle())
		return true;
	return true;
}

void	Who::execute(const Message& msg) {

	if (validate(msg))
	{
		/* Check permissions for execution of function */

	}

}