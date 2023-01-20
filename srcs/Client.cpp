#include "Client.hpp"
#include "Server.hpp"
#include "defines.h"

/* Constructors & Destructor */
Client::Client(int socket) : _socket(socket) {
	_isRegistered = false;
	_globalModes = 0;
}

Client::~Client() {
	shutdown(_socket, SHUT_RDWR);
}



/************************/
/*    Mode Management   */
/************************/

/* Set global server mode flags */
void	Client::setGlobalModes(const char &modes, bool removeMode) {
	/* removeMode toggles whether the provided mode(s) need(s) to be added or removed */
	if (removeMode)
	{
		_globalModes &= ~(modes);
		return ;
	}
	_globalModes |= modes;
}

/* Check global server mode flags */
bool	Client::checkGlobalModes(const char &modes) const {
	return _globalModes & modes;
}


/*****************************/
/*      I/O Management       */
/*****************************/

/* Read data from socket */
void	Client::read(void) {
    char		buf[MAX_BUFFER_SIZE];
	int			nbytes;
	std::string	input;

	/* Read from client socket */
	nbytes = recv(_socket, buf, MAX_BUFFER_SIZE, 0);
	if (nbytes < 0)
	{
		// FIXME: Handle recv error
		std::cerr << "recv() call failed on socket #" << _socket << ".\n"
				  << "errno : " << errno << std::endl;
	}
	//Testing fix for weird chars being read (null terminate before attempting to convert to std::string)
	buf[nbytes] = '\0';
	input = buf;
	if (nbytes == MAX_BUFFER_SIZE)
		input = input.replace(510, 2, "\r\n");
	_inputBuffer.append(input);

	/* Print received messages from client */
	if (DEBUG)
	{
		std::cerr << BLUE "Message received from client on socket #" << _socket << CLEAR << std::endl <<
				"	" << input << std::endl;
	}
}


/* Send data to client through socket */
void	Client::reply(const std::string& reply) {
	size_t sz;
	
	if (DEBUG)
		std::cerr << RED"Sending reply to client on socket #" << _socket << CLEAR << std::endl <<
			"	" << reply << std::endl;
	
	if ((sz = send(_socket, reply.c_str(), reply.size(), 0)) < 0)
		throw std::runtime_error("Error sending message");
	std::cerr << RED "return from send() on socket #" << _socket << ": " << sz << CLEAR << std::endl;

}

/* Check input buffer and return a single message command string */
std::string	Client::retrieveMessage() {
	size_t			it;
	std::string		rawMessage;
	
    /* If there remains text after the "\r\n", store it. */
    it = _inputBuffer.find("\r\n");
    if (it != std::string::npos)
	{
        rawMessage = _inputBuffer.substr(0, it + 2);
        _inputBuffer = _inputBuffer.substr(it + 2, _inputBuffer.size());
    }

	return rawMessage;
}
