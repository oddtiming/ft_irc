#ifndef AWAY_HPP
#define AWAY_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class Away : public Command
{
	public:
		/* Constructors & Destructor */
		Away(Server* server);
		~Away();

		/* Public Member Functions */
		bool                validate(const Message& msg);
		void                execute(const Message& msg);

	private:
		Client*		_client;

};

#endif