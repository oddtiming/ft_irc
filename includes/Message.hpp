#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "User.hpp"

class Message {
	public:
		/* Constructors & Destructor */
		Message();
		~Message();

		/* Operator Overloads */

		/* Setters & Getters */

		/* Public Member Functions */

	private:
		std::string	_rawMessage;
		User*		_owner;

};

#endif