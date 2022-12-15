#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join(Server* server) : Command("join", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */
bool	Join::validate(const Message& msg) {

std::map<std::string, std::string> 	channels;
std::string token;








}

void	Join::execute(const Message& msg) {
	if (validate(msg))
	{
		/* Check if channel exists */
		
		/* If channel doesn't exist, set create and set user as admin */


	}
}

