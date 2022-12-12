#include "User.hpp"
#include "Server.hpp"

#define MAX_BUFFER_SIZE 512
//This should be addressed

/* Constructors & Destructor */
User::User(int sockFD) : _socket(sockFD) {
	
}

User::~User() {
	shutdown(_socket, SHUT_RDWR);
}


/* Operator Overloads */

/* Setters & Getters */

/* Public Member Functions */

Message	User::read(void) {
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

void	User::reply(const std::string& msg) {
	if (send(_socket, msg.c_str(), sizeof(msg), 0) < 0)
		throw std::runtime_error("Error sending message");
}
