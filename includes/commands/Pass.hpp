#ifndef PASS_HPP
#define PASS_HPP

#pragma once

/* System Includes */
#include <string>
#include <exception>

/* Local Includes */
#include "Command.hpp"

class Pass : public Command {
    public:
        /* Constructors & Destructor */
        Pass(Server *server);
        ~Pass() {}

        /* Public Member Functions */
		bool						validate(const Message& msg);
		void						execute(const Message& msg);

		class passException : public std::exception {
			public:
				const char* what() const throw() {
					return ("Incorrect password");
				}
		};


	private:
		Client*						_client;
		const std::string			_password;
};

#endif