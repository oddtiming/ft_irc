#ifndef BOT_HPP
#define BOT_HPP

#pragma once

/* System Includes */
#include <curl/curl.h> // CURL*, CURLcode, curl_slist*, curl_slist_append(), curl_easy_setopt(), curl_easy_perform(), curl_easy_strerror(), curl_easy_cleanup()

#include <arpa/inet.h>  // inet_pton(), htons()
#include <cstdlib>      // getenv()
#include <ctime>        // time()
#include <iostream>     // cout, cerr, endl
#include <netinet/in.h> // struct sockaddr_in
#include <sstream>      // istringstream(), ostringstream()
#include <string> // find(), find_[first/last]_[not_]of(), size(), append(), substr(), insert(), c_str()
#include <sys/socket.h> // send()
#include <unistd.h>

using std::string;

class Bot
{
  public:
	Bot(string name, string servName, string channel, int port, string pass = string( ));
	virtual ~Bot( );

	/* To be overloaded by children classes */
	virtual int reply(std::string command) = 0;

	/* Getters */
	const std::string &getName( ) const;
	const int         &getSock( ) const;

	/* Public Member Functions */
	int connect( );

  protected:
	/* Inherited Member Functions */
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	int           sendChannelMessage( );

	/* Inherited Member Attributes */
	std::string _name;     // Name of the Bot
	std::string _servName; // printable ip address of the target server
	std::string _channel;  // Active channel to send to
	int         _port;     // Connection port of the target server
	int         _sock;     // Socket of the established connection
	std::string _password; // Optional. Server password.
	std::string _reply;    // the reply message that will be sent to the channel
};

#endif