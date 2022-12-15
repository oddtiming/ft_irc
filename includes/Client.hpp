#ifndef CLIENT_HPP
# define CLIENT_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>
#include <sys/socket.h>

/* Local Includes */
#include "Channel.hpp"
#include "Message.hpp"

class Client {
	public:
		/* Constructors & Destructor */
		Client(int socket);
		~Client();

		/* Operator Overloads */

		/* Setters & Getters */
		void				setNickname(const std::string& nickname) { _nickname = nickname; }
		void				setPassword(const std::string& password) { _password = password; }
		void				setUsername(const std::string& username) { _username = username; }
		void				setRealname(const std::string& realname) { _realname = realname; }

		const std::string&	getNickname(void) const { return (_nickname); }
		const std::string&	getPassword(void) const { return (_password); }
		const std::string&	getUsername(void) const { return (_username); }
		const std::string&	getRealname(void) const { return (_realname); }
		const int&			getSocket(void) const { return (_socket); }

		void				removeChannelModes(const std::string& channel);
		void				modifyChannelModes(const std::string& channelName, const char& mode, bool removeMode = false);
		bool				checkChannelModes(const std::string& channelName, const char& mode) const;

		void				modifyGlobalModes(const char& mode, bool removeMode = false);
		bool				checkGlobalModes(const char& mode) const;

		/* Public Member Functions */
		void	read();
		void	reply(const std::string& reply);

		void	replyAllChannels(const std::string& reply);

		std::string	retrieveMessage();


		class notChannelMemberException : public std::exception {
		public:
			const char*	what() const throw() {
				return("notChannelMemberException: Client is not part of this channel");
			}
		};

	private:
		const int						_socket;
		std::string						_nickname;
		std::string						_username;
		std::string						_realname;
		std::string						_password;
		char							_globalModes;	/* Mode flags stored using bitmask */
		std::map<std::string, char>		_channelModes;	/* Per client channel modes */

		std::string						_inputBuffer;	/*  Raw data received from socket reads */
		std::string						_awayMessage;
		// bool							_isAway;
		
		/* Private Member Functions */
};

#endif