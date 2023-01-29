#include "Client.hpp"
#include "Server.hpp"
#include "defines.h"

/* Constructors & Destructor */
Client::Client(int socket) : _socket(socket), _timeConnect(std::time(nullptr)) {
	_isPassValidated = false;
	_isRegistered    = false;
	_globalModes     = 0;
	_wasPinged       = false;
}

Client::~Client( ) { shutdown(_socket, SHUT_RDWR); }

/************************/
/*    Mode Management   */
/************************/

/* Set global server mode flags */
void Client::setGlobalModes(const char &modes, bool removeMode) {
	/* removeMode toggles whether the provided mode(s) need(s) to be added or removed */
	if (removeMode) {
		_globalModes &= ~(modes);
		return;
	}
	_globalModes |= modes;
}

/* Check global server mode flags */
bool Client::checkGlobalModes(const char &modes) const { return _globalModes & modes; }

std::string Client::getGlobalModes(void) {
	std::string globalModes = "";
	if (checkGlobalModes(OP))
		globalModes += 'o';
	if (checkGlobalModes(AWAY))
		globalModes += 'a';
	if (checkGlobalModes(INVIS))
		globalModes += 'i';
	return globalModes;
}

/*****************************/
/*      I/O Management       */
/*****************************/

/* Read data from socket */
int Client::read(void) {
	char        buf[MAX_BUFFER_SIZE];
	int         nbytes;
	std::string input;

	/* Read from client socket */
	nbytes = recv(_socket, buf, MAX_BUFFER_SIZE, 0);
	/* If return is 0 or less, client has disconnected */
	if (nbytes <= 0)
		return (nbytes);
	buf[nbytes] = '\0';
	input       = buf;
	if (nbytes == MAX_BUFFER_SIZE)
		input = input.replace(510, 2, "\r\n");
	_inputBuffer.append(input);
	_timeLastActivity = std::time(nullptr);
	/* Print received messages from client */
	std::cout << getTimestamp( ) << BLUE "Raw input received from client on socket #"
	          << _socket << ":" << CLEAR << std::endl;

	/* Print input to terminal in proper format */
	std::string tmp = _inputBuffer;
	size_t      pos = 0;
	while ((pos = tmp.find("\n")) != std::string::npos) {
		std::cout << "\t\t\t\t" << tmp.substr(0, pos + 1);
		tmp.erase(0, pos + 1);
	}
	std::cout << "\t\t\t\t" << tmp << std::endl;
	return (nbytes);
}

/* Send data to client through socket */
void Client::reply(const std::string &reply) {
	size_t sz;

	std::cerr << getTimestamp( ) << RED "Sending reply to client on socket #" << _socket
	          << ":" << CLEAR << std::endl;
	std::cout << "\t\t\t\t" << reply << std::endl;

	if ((sz = send(_socket, reply.c_str( ), reply.size( ), 0)) < 0)
		throw std::runtime_error("Error sending message");
	// std::cerr << RED "return from send() on socket #" << _socket << ": " << sz << CLEAR
	// << std::endl;
}

/* Check input buffer and return a single message command string */
std::string Client::retrieveMessage( ) {
	size_t      it;
	std::string rawMessage;

	/* If there remains text after the "\r\n", store it. */
	it = _inputBuffer.find("\r\n");
	if (it != std::string::npos) {
		rawMessage   = _inputBuffer.substr(0, it + 2);
		_inputBuffer = _inputBuffer.substr(it + 2, _inputBuffer.size( ));
	}

	return rawMessage;
}

const std::string Client::getAddress( ) const {
	char buf[INET_ADDRSTRLEN + 1];

	if (inet_ntop(AF_INET, &_address.sin_addr, buf, sizeof(buf)) != NULL)
		return ("127.0.0.1");
	else
		return std::string(buf);
}