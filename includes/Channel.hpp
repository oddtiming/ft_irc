#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#pragma once

/* System Includes */
#include <map>
#include <string>

/* Local Includes */
#include "Client.hpp"

class Channel {
	public:
		Channel(const std::string& name, const std::string& pass, Client* owner);
		~Channel();

		void	closeChannel();

		/*************************/
		/*   Setters & Getters   */
		/*************************/
		const	std::string& getName() { return (_name); }
		const	std::string& getTopic() { return (_topic); }
		void	setTopic(const std::string topic) { _topic = topic; }

		void	setOwner(Client* owner) { _owner = owner; }
		Client*	getOwner(void) const { return (_owner); }

		std::string	getPass() { return (_pass); }
		void	setPass(const std::string& pass) { _pass = pass; }

		/*******************************/
		/*        Mode Management      */
		/*******************************/
		void	setModes(char modes, bool isAdd);
		void	setMemberModes(const std::string& nick, char modes, bool isAdd);
		bool	checkModes(char modes);
		bool	checkMemberModes(const std::string& nick, char modes);

		/***********************************/
		/*    Channel Member Management    */
		/***********************************/
		bool	isMember(const std::string& nick);
		void	addMember(Client* client);
		void	removeMember(const std::string& nick);

	private:
		const std::string				_name;			/* Channel name */
		std::string						_pass;
		std::string						_topic;			/* Channel topic */
		Client *						_owner;			/* Channel owner */
		std::map<std::string, Client *>	_members;		/* Channel member list */
		std::map<std::string, char>		_memberModes;	/* Per client channel modes */
		char							_modes;			/* Channel modes */
};

#endif
