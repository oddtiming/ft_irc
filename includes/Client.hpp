#ifndef CLIENT_HPP
# define CLIENT_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
/* Local Includes */
#include "Channel.hpp"
#include "Message.hpp"

class Client {
	public:
		Client(int socket);
		~Client();

		/*************************/
		/*   Setters & Getters   */
		/*************************/
		void				setNickname(const std::string& nickname) { _nickname = nickname; }
		void				setPassword(const std::string& password) { _password = password; }
		void				setUsername(const std::string& username) { _username = username; }
		void				setRealname(const std::string& realname) { _realname = realname; }
		void				setHostname(const std::string& hostname) { _hostname = hostname; }
		void				setAddress(const struct sockaddr_in address) { _address = address; }
		void				setRegistration(const bool& registration) { _isRegistered = registration; }
		void 				setAwayMessage(const std::string& awayMessage) {_awayMessage = awayMessage;}
		void				setPingStatus(const bool& status) { _wasPinged = status; }

		const std::string&	getNickname(void) const { return (_nickname); }
		const std::string&	getPassword(void) const { return (_password); }
		const std::string&	getUsername(void) const { return (_username); }
		const std::string&	getRealname(void) const { return (_realname); }
		const std::string&	getHostname(void) const { return (_hostname); }
		const std::string&	getAwayMessage(void) const { return (_awayMessage); }
		const int&			getSocket(void) const { return (_socket); }
		bool				getRegistration(void) { return (_isRegistered); }
		const std::time_t&	getConnectTime(void) const { return (_timeConnect); }
		const std::time_t&	getLastActivityTime(void) const { return _timeLastActivity; }
		const bool&			getPingStatus(void) const { return _wasPinged; }
		
		/*************************/
		/*    Mode Management    */
		/*************************/
		void				setGlobalModes(const char& mode, bool removeMode = false);
		bool				checkGlobalModes(const char& mode) const;

		/************************/
		/*    I/O Management    */
		/************************/
		void				read();
		void				reply(const std::string& reply);
		std::string			retrieveMessage();


	private:
		const int						_socket;
		std::string						_nickname;
		std::string						_username;
		std::string						_realname;
		std::string						_password;
		char							_globalModes;		/* Mode flags stored using bitmask */
		std::string						_inputBuffer;		/* Raw data received from socket reads */
		std::string						_awayMessage;
		// bool							_isAway;
		bool							_isRegistered;


		/* Time management */
		const std::time_t				_timeConnect;
		std::time_t						_timeLastActivity;
		bool							_wasPinged;

		struct sockaddr_in				_address;
		std::string						_hostname;
		
		/* Private Member Functions */
};

#endif