#ifndef TIMEBOT_HPP
# define TIMEBOT_HPP

#pragma once

/* System Includes */
#include <iostream>
#include <ctime>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>


class TimeBot {
	public:
		TimeBot();
		~TimeBot();


	private:
		int						_port;
		int						_socket;
		std::string				_channel;
		std::string				_password;
		struct sockaddr_in		_serverAddress;
};

#endif