/* Local Includes */
#include "Server.hpp"

int	main(int argc, char **argv)	{

	/* Send usage error message if parameters are not correct */
	if (argc != 3)
		std::cerr << "usage: " << argv[0] << " <port> <password>" << std::endl;
	/* Start server with given parameters */
	else
	{
		std::string servername = argv[0];
		servername.erase(0, 2);
		Server server(servername, atoi(argv[1]), argv[2]);
	}
}