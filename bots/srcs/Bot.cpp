/* Local Includes */
#include "Bot.hpp"

/**************************/
/* Constructor/Destructor */
/**************************/
Bot::Bot(string name, string servName, string channel, int port, string pass)
  : _name(name), _servName(servName), _channel(channel), _port(port), _password(pass) {}

Bot::~Bot( ) {}

/***************************/
/* Public member functions */
/***************************/

/**
 * @return int: socket number on success, -1 on failure
 */
int Bot::connect( ) {
	// Create the socket
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == -1) {
		std::cerr << "Error creating socket" << std::endl;
		return -1;
	}

	// Connect to the server
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port   = htons(_port);
	inet_pton(AF_INET, _servName.c_str( ), &server_address.sin_addr);
	if (::connect(_sock, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Error connecting to server" << std::endl;
		return -1;
	}

	// Send the server password if it is set
	if (!_password.empty( )) {
		std::string message = "PASS " + _password + "\r\n";
		if (send(_sock, message.c_str( ), message.size( ), 0) == -1) {
			std::cerr << "Error sending server password" << std::endl;
			return -1;
		}
	}

	// Send the nickname
	string message = "NICK " + _name + "\r\n";
	if (send(_sock, message.c_str( ), message.size( ), 0) == -1) {
		std::cerr << "Error sending nickname" << std::endl;
		return -1;
	}

	// Send the username
	message = "USER " + _name + " 0 * :" + _name + "\r\n";
	if (send(_sock, message.c_str( ), message.size( ), 0) == -1) {
		std::cerr << "Error sending username" << std::endl;
		return -1;
	}

	// Verify that the server accepted the connection and sent the 001 RPL_WELCOME
	char buffer[4096];
	int  bytes_received = recv(_sock, buffer, sizeof(buffer), 0);
	if (bytes_received == -1 || ((message = buffer).find("001") == string::npos)) {
		std::cerr << "Error establishing server connection" << std::endl;
		return -1;
	}

	// Join the channel
	message = "JOIN " + _channel + "\r\n";
	if (send(_sock, message.c_str( ), message.size( ), 0) == -1) {
		std::cerr << "Error joining channel" << std::endl;
		return -1;
	}

	return _sock;
}

/******************************/
/* Inherited member functions */
/******************************/

size_t Bot::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

int Bot::sendChannelMessage( ) {
	_reply = "PRIVMSG " + _channel + " :" + _reply + "\r\n";
	std::cerr << "Sending _reply : '" << _reply << "'" << std::endl;
	if (send(_sock, _reply.c_str( ), _reply.size( ), 0) == -1) {
		std::cerr << "Error sending _reply" << std::endl;
		return 1;
	}
	return 0;
}

/***********/
/* Getters */
/***********/

const std::string &Bot::getName( ) const { return this->_name; }
const int         &Bot::getSock( ) const { return this->_sock; }
