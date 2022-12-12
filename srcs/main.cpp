
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

	if (argc != 3)
		std::cerr << "usage: " << argv[0] << " <port> <password>" << std::endl;

	Server server("ircserv", atoi(argv[1]), argv[2]);
}
