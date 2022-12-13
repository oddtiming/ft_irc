#include "commands/Nick.hpp"

/* Constructors & Destructor */
Nick::Nick() : Command("join") {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Nick::~Nick() {
}

/* Public Member Functions */
bool	Nick::validate(const Message& msg) {

}

const std::string&	Nick::execute(const Message& msg) {

	/* Check permissions for execution of function */
}
