#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join() : Command("join") {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */

const std::string&	Join::execute(const Message& msg) {

	/* Check permissions for execution of function */

}