#ifndef COMMAND_HPP
# define COMMAND_HPP

#pragma once

/* Systems Includes */
#include <string>

/* Local Includes */
#include "Message.hpp"

class Command {
	public:
		/* Constructors & Destructor */
		
		virtual ~Command();

		/* Operator Overloads */

		/* Setters & Getters */

		/* Public Member Functions */
		virtual const std::string&	execute(const Message& msg) const = 0;

	protected:
		Command(const std::string& name);
		const std::string	_name;
		bool				_channelOpRequired;
		bool				_globalOpRequired;
		int					_replCode;

};

#endif