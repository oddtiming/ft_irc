#ifndef CLIENT_HPP
# define CLIENT_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

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
		void				setAddress(const struct sockaddr_in address) { _address = address; }

		const std::string&	getNickname(void) const { return (_nickname); }
		const std::string&	getPassword(void) const { return (_password); }
		const std::string&	getUsername(void) const { return (_username); }
		const std::string&	getRealname(void) const { return (_realname); }
		const int&			getSocket(void) const { return (_socket); }
		
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

		struct sockaddr_in				_address;
		
		/* Private Member Functions */
};

#endif