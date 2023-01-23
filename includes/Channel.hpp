#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#pragma once

/* System Includes */
#include <map>
#include <vector>
#include <stdint.h>		// uint32_t
#include <string>
#include <ctime>
#include <iostream>

/* Class Definitions */
class Client;

class Channel {
	
	
	public:
	/* Members are stored with their addresses as the key, and their memberModes as the value */
		typedef uint32_t						Mode;
		typedef std::map<Client *, Mode>		MemberMap;

		Channel(const std::string& name, Client* owner);
		~Channel();

		void	closeChannel();

		/*************************/
		/*   Setters & Getters   */
		/*************************/
		const	std::string& getName() { return (_name); }
		const	std::string& getTopic() { return (_topic); }
		void	setTopic(const std::string topic) { _topic = topic; }
		void	setOwner(Client* owner) { _owner = owner; }
		void	setPass(const std::string& pass) { _pass = pass; }

		Client*		getOwner(void) const { return (_owner); }
		std::string	getPass() { return (_pass); }
		int			getHighestRank(Client* client);
		const std::time_t&	getStartTime(void) const { return _timeStart; }

		/*******************************/
		/*        Mode Management      */
		/*******************************/
		void	setModes(char modes, bool removeMode = false);
		void	setMemberModes(Client* client, char modes, bool removeMode = false);
		bool	checkModes(char modes);
		bool	checkMemberModes(Client *client, char modes);
		std::string 		getChannelModes(void);


	/***********************************/
		/*    Channel Member Management    */
		/***********************************/
		bool	isMember(Client* client);
		void	addMember(Client* client, const std::string& reply, int modes = 0);
		void	removeMember(Client* client, const std::string& reply);
		void	ensureOperator(void);
		std::string getMemberList(void);
		std::vector<std::string> getMemberVector(void);

		/* Channel Messaging */

		void	sendToAll(const std::string& reply);
		void	sendToOthers(const std::string& reply, Client* sender);



private:
		const std::string				_name;			/* Channel name */
		std::string						_pass;
		std::string						_topic;			/* Channel topic */
		Client *						_owner;			/* Channel owner */
		MemberMap						_members;		/* Channel member list, paired with their memberModes */
		MemberMap						_notMembers;	/* Clients with flags set who are not currently in the channel */
		char							_modes;			/* Channel modes */
		const std::time_t				_timeStart;		/* Time channel was created */



};

#endif
