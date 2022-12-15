#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#pragma once

/* System Includes */
#include <vector>
#include <string>

/* Local Includes */

class Client;

class Channel {
	public:
		/* Constructors & Destructor */
		Channel() { }
		~Channel() { }
		/* Operator Overloads */

		/* Setters & Getters */
		// const	std::string& getName();
		// void	setName(const std::string name);

		// const	std::string& getTopic();
		// void	setTopic(const std::string name);

		// void	setOwner(const Client* client);
		// void	changeOwner(const Client* client);
		
		// bool	isOp(const Client& client);
		// void	addOp(const Client* client);
		// void	removeOp(const Client* client);

		// bool	isMember(const Client& client);
		// void	addMember(const Client* client);
		// void	removeMember(const Client* client);

		// void	setModes(char modes);

		/* Public Member Functions */

	private:
		// std::string						_name;			/* Channel name */
		// std::string						_topic;			/* Channel topic */
		// Client *						_owner;			/* Channel owner */
		// std::map<std::string, Client *>	_members;		/* Channel member list */
		// char							_modes;			/* Channel modes */
};

#endif