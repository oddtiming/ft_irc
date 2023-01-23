#include "../../includes/commands/Mode.hpp"

Mode::Mode(Server* server) : Command("mode", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Mode::~Mode() {

}

bool	Mode::validate(const Message& msg) {
	std::vector<std::string> middle = msg.getMiddle();


	/*check if there's a target for the command*/
	//fixme: when receiving "MODE #channelName", reply with channel modes
	if (middle.size() < 1) {
		msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
		return false;
	}
	std::string target = middle.at(0);
	bool removeMode;
	std::string modes = "+-";


	if (target.at(0) == '#')
		_targetIsChannel = 1;
	else
		_targetIsChannel = 0;
	if (middle.size() == 1 && _targetIsChannel)
	{
		msg._client->reply(RPL_CHANNELMODEIS(target, "+", _server->getChannelPtr(target)->getChannelModes()));
		return false;
	}
	//fixme: add code to send all modes to client in case of /mode #targetChannel
	if (middle.size() > 1) {
		std::string mode = middle.at(1);
		if (mode.c_str()[0] == '-')
			removeMode = true;
		else if (mode.c_str()[0] == '+')
			removeMode = false;
		else {
			msg._client->reply(ERR_UNKNOWNMODE(mode, target));
			return false;
		}
		if (_targetIsChannel == 0){
			if (mode.c_str()[1] == 'o') {
				msg._client->setGlobalModes(OP, removeMode);
				msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
			} else if (mode.c_str()[1] == 'i') {
				msg._client->setGlobalModes(INVIS, removeMode);
				msg._client->reply(RPL_UMODEIS(msg._client->getGlobalModes()));
			} else {
				msg._client->reply(ERR_UNKNOWNMODE(mode, target));
				return false;
			}
		}
		if (_targetIsChannel ==
			1/* && (msg._client->checkGlobalModes(OP) || _server->getChannelPtr(target)->checkMemberModes(msg._client,C_OP ))*/) {
			{

				if (mode.c_str()[1] == 'p') {
					_server->getChannelPtr(target)->setModes(PRIVATE, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 's') {
					_server->getChannelPtr(target)->setModes(SECRET, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'm') {
					_server->getChannelPtr(target)->setModes(MODERATED, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'i') {
					_server->getChannelPtr(target)->setModes(INV_ONLY, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 't') {
					_server->getChannelPtr(target)->setModes(TOPIC_SET_OP, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'n') {
					_server->getChannelPtr(target)->setModes(NO_MSG_IN, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}
				if (mode.c_str()[1] == 'k') {
					_server->getChannelPtr(target)->setModes(PASS_REQ, removeMode);
					msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
				}

				if (middle.size() > 2) {
					std::string user = middle.at(2);
					if (mode.c_str()[1] == 'b') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), BAN, removeMode);
						if (_server->getChannelPtr(target)->isMember(_server->getClientPtr(user)))
							_server->getChannelPtr(target)->removeMember(_server->getClientPtr(user),
																		 CMD_PART(user, target, "User has been banned"));
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
					if (mode.c_str()[1] == 'i') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), INV, removeMode);
						// msg._client->reply(RPL_INVITING(target, user));
					}
					if (mode.c_str()[1] == 'v') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), VOICE, removeMode);
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
					if (mode.c_str()[1] == 'o') {
						_server->getChannelPtr(target)->setMemberModes(_server->getClientPtr(user), C_OP, removeMode);
						msg._client->reply(RPL_CHANNELMODEIS(target, modes.at(removeMode), mode.c_str()[1]));
					}
				}
				else {
					msg._client->reply(ERR_UNKNOWNMODE(mode, target));
					return false;
				}
			}

		}
	}
	return true;
}

void	Mode::execute(const Message& msg) {
	if (validate(msg)){

	}
}
