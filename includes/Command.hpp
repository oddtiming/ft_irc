#ifndef COMMAND_HPP
# define COMMAND_HPP

#pragma once

/* Systems Includes */
#include <string>

/* Local Includes */


class Command {
	public:
		/* Constructors & Destructor */
		Command(const std::string& name);
		~Command();

		/* Operator Overloads */

		/* Setters & Getters */

		/* Public Member Functions */

	private:
		const std::string	_name;
		bool				_channelOpRequired;
		bool				_globalOpRequired;
		int					_replCode;

};

#endif