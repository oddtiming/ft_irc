/* Local includes */
#include "DictBot.hpp"
#include "JokeBot.hpp"
#include "TimeBot.hpp"

/* System includes */
#include <iostream> // cout(), cerr(), endl()
#include <sstream>  // stringstream
#include <string> // find(), find_[first/last]_[not_]of(), size(), append(), substr(), insert(), c_str()
#include <sys/socket.h> // recv()

const char       *IP_ADDR         = "127.0.0.1";
const std::string CHANNEL         = "#bots";
const int         PORT            = 6667;
const std::string SERVER_PASSWORD = "pass";

static inline int runBot(Bot *bot);

int main(int argc, char **argv) {
	if (argc < 2
	    || (strcmp(argv[1], "dict") && strcmp(argv[1], "time")
	        && strcmp(argv[1], "joke"))) {
		std::cerr << "usage: " << argv[0] << " ['dict'/'time'/'joke']" << std::endl;
		return 1;
	}
	const std::string command = argv[1];
	std::string       botName = command + "Bot";
	Bot	          *bot;

	// Determine which type of bot this instance is
	switch (*argv[1]) {
	case 'd':
		bot = new DictBot(botName, IP_ADDR, CHANNEL, PORT, SERVER_PASSWORD);
		break;
	case 'j':
		bot = new JokeBot(botName, IP_ADDR, CHANNEL, PORT, SERVER_PASSWORD);
		break;
	case 't':
		bot = new TimeBot(botName, IP_ADDR, CHANNEL, PORT, SERVER_PASSWORD);
		break;
	}

	int sock = bot->connect( );
	if (sock < 0)
		return 1;

	// Continuously read and process messages from the server
	while (true) {
		if (runBot(bot)) {
			delete bot;
			return 1;
		}
	}
	return 0;
}

static inline int runBot(Bot *bot) {
	char buffer[4096];
	int  bytes_received = recv(bot->getSock( ), buffer, sizeof(buffer), 0);
	if (bytes_received == -1) {
		std::cerr << "Error receiving message" << std::endl;
		return 1;
	}
	// Convert the buffer to a string
	std::string message(buffer, bytes_received);
	// Split the message into lines
	std::stringstream stream(message);
	std::string       line;

	// Check if the message is a command
	while (getline(stream, line, '\r')) {
		std::cout << "Received: " << line << std::endl;
		size_t it = line.find("PRIVMSG " + CHANNEL);
		if (it != std::string::npos)
			it = line.find(" :", it);
		if (it != std::string::npos && line.at(it + 2) == '!') {
			std::string userCommand = line.substr(it + 3);
			std::cout << "userCommand = !" << userCommand << std::endl;

			// Check if userCommand is the the same as the bot prefix
			if (!strncmp(userCommand.c_str( ), bot->getName( ).c_str( ), 4)) {
				int ret = bot->reply(userCommand);
				if (ret) {
					std::cerr << "Error sending message, exiting" << std::endl;
					return 1;
				}
			}
			else {
				std::cerr << "Did not recognize command" << std::endl;
			}
		}
		// Respond to server's pings
		else if (line.find("PING")
		         && send(bot->getSock( ), "PONG localhost\r\n", 16, 0) == -1) {
			std::cerr << "Error sending message, exiting" << std::endl;
			return 1;
		}
	}
	return 0;
}
