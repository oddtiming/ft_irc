#include "commands/Ban.hpp"

Ban::Ban(Server* server) : Command("ban", server) {
	_channelOpRequired = true;
	_globalOpRequired = false;
}

Ban::~Ban() {

}

bool	Ban::validate(const Message& msg) {

}