#include "commands/Ping.hpp"

/* Constructors & Destructor */
Ping::Ping() : Command("ping") {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Ping::~Ping() {
}

/* Public Member Functions */
bool	Ping::validate(const Message& msg) {

}


const std::string&	Ping::execute(const Message& msg) {

	/* Check permissions for execution of function */
}