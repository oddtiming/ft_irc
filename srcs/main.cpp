
/* System Includes */
#include <iostream>
#include <string>
#include <cstring>

/* Client Includes */
#include "Server.hpp"

int	main(int argc, char **argv)	{

	if (argc != 3)
		std::cerr << "usage: " << argv[0] << " <port> <password>" << std::endl;
	else
		Server server("ircserv", atoi(argv[1]), argv[2]);
}
