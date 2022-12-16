#include "commands/Kick.hpp"

Kick::Kick(Server* server) : Command("kick", server) {
	_channelOpRequired = true;
	_globalOpRequired = false;
}


Kick::~Kick() {

}
