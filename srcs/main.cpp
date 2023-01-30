/* Local Includes */
#include "Server.hpp"

int g_status = OFFLINE;

void	sig_terminate(int signum) {
	(void) signum;
	std::cerr << "SIGINT received, turning off the server." << std::endl;
	g_status = OFFLINE;
}

int	main(int argc, char **argv)	{
	/* Send usage error message if parameters are not correct */
	if (argc != 3)
		std::cerr << "usage: " << argv[0] << " <port> <password>" << std::endl;

	/* Start server with given parameters */
	else
	{
		signal(SIGINT, sig_terminate);
		std::string servername = argv[0];
		servername.erase(0, 2);
		try {
			Server server(servername, atoi(argv[1]), argv[2]);
		} catch (std::runtime_error &e) {
			std::cerr << "Caught runtime error of type " << e.what() 
				      << ". Exiting program" << std::endl;
			return 1;
		}
	}
	return 0;
}
