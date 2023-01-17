#include "../../includes/commands/Mode.hpp"

Mode::Mode(Server* server) : Command("mode", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Mode::~Mode() {

}

bool	Mode::validate(const Message& msg) {
	std::vector<std::string>	middle = msg.getMiddle();


	/*check if there's a target for the command*/
	//fixme: when receiving "MODE #channelName", reply with channel modes
	if (middle.size() < 2)
	{
		/*msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;*/
		return false;
	}
	std::string					target = middle.at(0);
	std::string					mode = middle.at(1);
	bool						removeMode;

	if (mode.c_str()[0] == '-')
		removeMode = true;
	else if (mode.c_str()[0] == '+')
		removeMode = false;
	else {
		msg._client->reply(ERR_UNKNOWNMODE(mode, target));
		return false;
	}
	if (mode.c_str()[1] == 'o')
		msg._client->setGlobalModes(C_OP, removeMode);
	else
	{
		msg._client->reply(ERR_UNKNOWNMODE(mode, target));
		return false;
	}

	return true;

}

void	Mode::execute(const Message& msg) {
	if (validate(msg)){
		msg._client->reply("1 alx Changed mode to +o");
		if (msg._client->checkGlobalModes(C_OP))
			msg._client->reply(" and it worked!\r\n");
	}
}
