
/* System Includes */
#include <iostream>
#include <string>
#include <cstring>

/* User Includes */
#include "Server.hpp"


class badArgsException : public std::exception {
	public:
		const char*	what() const throw() {
			return("usage: ./ircserv <port> <password>");
		}
};

int	main(int argc, char **argv)	{

	try {
		if (argc != 3)
			throw badArgsException();
		Server server("ircserv", atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}	
}