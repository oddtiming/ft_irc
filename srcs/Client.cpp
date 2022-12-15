#include "Client.hpp"
#include "Server.hpp"
#include "defines.h"

#define MAX_BUFFER_SIZE 512
//This should be addressed

/* Constructors & Destructor */
Client::Client(int socket) : _socket(socket) {
	
}

Client::~Client() {
	shutdown(_socket, SHUT_RDWR);
}


/* Operator Overloads */

/* Setters & Getters */

/* Public Member Functions */

/**
 * WARNING: throws a notChannelMemberException if the Client is not part of the Channel
 */
void	Client::modifyChannelModes(const std::string& channelName, const char& mode, bool removeMode) {
	std::map<std::string, char>::iterator	it;

	/* Check if the Client is a member of the channel */
	it = _channelModes.find(channelName);
	if (it == _channelModes.end())
		throw notChannelMemberException();
	/* removeMode toggles whether the provided mode(s) need(s) to be added or removed */
	if (removeMode)
	{
		(*it).second &= ~(mode);
		return ;
	}
	(*it).second |= mode;
}

/**
 * WARNING: throws a notChannelMemberException if the Client is not part of the Channel
*/
bool	Client::checkChannelModes(const std::string& channelName, const char& mode) const {
	std::map<std::string, char>::const_iterator	it;

	/* Check if the Client is a member of the channel */
	it = _channelModes.find(channelName);
	if (it == _channelModes.end())
		throw notChannelMemberException();
	return (*it).second & mode;
}

void	Client::modifyGlobalModes(const char &mode, bool removeMode) {
	/* removeMode toggles whether the provided mode(s) need(s) to be added or removed */
	if (removeMode)
	{
		_globalModes &= ~(mode);
		return ;
	}
	_globalModes |= mode;
}

bool	Client::checkGlobalModes(const char &mode) const {
	return _globalModes & mode;
}

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

void	Client::reply(const std::string& msg) {
	if (send(_socket, msg.c_str(), sizeof(msg), 0) < 0)
		throw std::runtime_error("Error sending message");
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
