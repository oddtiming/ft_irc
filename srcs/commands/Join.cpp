#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join() : Command("join") {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */
bool	Join::validate(const Message& msg) {

}

const std::string&	Join::execute(const Message& msg) {
	if (validate(msg)) {
		/* Perorm action */
	}
}

