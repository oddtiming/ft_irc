#include "Client.hpp"
#include "Server.hpp"

#define MAX_BUFFER_SIZE 512
//This should be addressed

/* Constructors & Destructor */
Client::Client(int sockFD) : _socket(sockFD) {
	
}

Client::~Client() {
	shutdown(_socket, SHUT_RDWR);
}


/* Operator Overloads */

/* Setters & Getters */

/* Public Member Functions */

Message	Client::read(void) {
	char	buf[MAX_BUFFER_SIZE];
	int		nbytes;

	/* Read from client socket */
	nbytes = recv(_socket, buf, MAX_BUFFER_SIZE, 0);
	if (nbytes < 0)
	{
		// FIXME: Handle recv error
		std::cerr << "recv() call failed on socket #" << _socket << ".\n"
				  << "errno : " << errno << std::endl;
	}
	return (Message(this, std::string(buf)));
}

void	Client::reply(const std::string& msg) {
	if (send(_socket, msg.c_str(), sizeof(msg), 0) < 0)
		throw std::runtime_error("Error sending message");
}


