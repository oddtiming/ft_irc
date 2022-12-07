#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#pragma once

/* System Includes */
#include <vector>
#include <string>

/* Local Includes */
#include "User.hpp"

class Channel {
	public:
		/* Constructors & Destructor */
		Channel();
		~Channel();
		/* Operator Overloads */

		/* Setters & Getters */
		const	std::string& getName();
		void	setName(const std::string name);

		const	std::string& getTopic();
		void	setTopic(const std::string name);

		void	setOwner(const User* user);
		void	changeOwner(const User* user);
		
		bool	isOp(const User& user);
		void	addOp(const User* user);
		void	removeOp(const User* user);

		bool	isMember(const User& user);
		void	addMember(const User* user);
		void	removeMember(const User* user);

		void	setModes(char modes);

		/* Public Member Functions */

	private:
		std::string			_name;			/* Channel name */
		std::string			_topic;			/* Channel topic */
		User *				_owner;			/* Channel ownder*/
		std::vector<User *>	_opList;		/* Channel operator list */
		std::vector<User *>	_members;		/* Channel member list */
		char				_modes;			/* Channel modes */
};

#endif