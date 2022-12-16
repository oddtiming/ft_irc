#include "Client.hpp"
#include "Server.hpp"
#include "defines.h"

/* Constructors & Destructor */
Client::Client(int socket) : _socket(socket) {
	
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
//FIXME: Check modes in channel and ensure that all of the bitwise operations are correctly implemented

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
	input = buf;
	if (nbytes == MAX_BUFFER_SIZE)
		input = input.replace(510, 2, "\r\n");
	_inputBuffer.append(input);
	if (DEBUG)
	{
		std::cerr << "Client #" << _socket << " received following message: '"
				<< input << "'." << std::endl;
	}
}


/* Send data to client through socket */
void	Client::reply(const std::string& reply) {
	size_t sz;
	
	// FIXME: debug
	std::cerr << "Sending reply '" << reply << "' to socket #" << _socket << std::endl;
	
	if ((sz = send(_socket, reply.c_str(), reply.size(), 0)) < 0)
		throw std::runtime_error("Error sending message");
		
	// FIXME: debug
	std::cerr << "return from send: " << sz << std::endl;
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
