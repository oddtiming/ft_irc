#include "User.hpp"

#define MAX_BUFFER_SIZE 512
//This should be addressed

/* Constructors & Destructor */
User::User(int sockFD) : _socket(sockFD) {
	
}


/* Operator Overloads */

/* Setters & Getters */

/* Public Member Functions */

Message*	User::read(void) {
	Message message(this);

	char buf[MAX_BUFFER_SIZE];
	/* Read from client socket */
	recv(this->_socket, &buf, sizeof(buf), 0);

	/* Convert to std::string and null terminate etc */

	return (&message);
}