
/* Local Includes */
#include "Channel.hpp"
#include "defines.h"


/* Constructors & Destructor */
Channel::Channel(const std::string& name, const std::string& pass, Client* owner) : _name(name), _owner(owner) {

	/* Set default channel modes */
	setModes(TOPIC_SET_OP | NO_MSG_IN);
	
	/* Add channel creator to member list, add OWNER to its modes */
	addMember(owner, OWNER);
}

Channel::~Channel() {

}

/* Close channel */
void	closeChannel() {
	//FIXME: Ensure all channel members receive proper notification
}

/*******************************/
/*        Mode Management      */
/*******************************/

/* Set channel wide mode flags */
void	Channel::setModes(char modes, bool removeMode) {
	if (removeMode) {
		_modes &= ~(modes);
		return ;
	}
	_modes |= modes;
}

/* Set member mode flags for a specified client */
void	Channel::setMemberModes(Client* client, char modes, bool removeMode) {
	MemberMap::iterator it = _members.find(client);
	
	if (it == _members.end())
	{
		/* If user not found in members, check whether their status is stored */
		it = _notMembers.find(client);
		if (it == _notMembers.end())
			return;
	}
	if (removeMode) {
		it->second &= ~(modes);
		if (modes & BAN) {
			_notMembers.erase(it);
		}
		return ;
	}
	it->second |= modes;
	
	// After member mode is set run KICK command on banned user.
	// TODO: MODE command will remove the client if the provided mode has "+b"
}

/* Check channel wide mode flags */
bool	Channel::checkModes(char modes) {
	return (_modes & modes) == modes;
}

/* Check member mode flags for specified client */
bool	Channel::checkMemberModes(Client* client, char modes) {
	MemberMap::iterator it = _members.find(client);
	
	/* Return without doing anything if user not found in list */
	if (it == _members.end()) {
		it = _notMembers.find(client);
		if (it == _notMembers.end())
			return;
	}
	return (it->second & modes) == modes;
}


/***********************************/
/*    Channel Member Management    */
/***********************************/

/* Check if specified client is a member of channel */
bool	Channel::isMember(Client* client) {
	return (_members.find(client) != _members.end());
}

/* Add a new member to channel */
void	Channel::addMember(Client* client, int modes) {
	/* Add member and set default member modes */
	_members[client] = modes; 
}

/* Remove a member from channel */
void	Channel::removeMember(Client* client) {
	bool wasOpe = false;
	MemberMap::iterator it = _members.find(client);

	/* Return is member not found in channel */
	if (it == _members.end())
		return;

	/* Check if member was owner */
	if (checkMemberModes(client, C_OP))
		wasOpe = true;

	/* If member is banned keep track of them*/
	if (checkMemberModes(client, BAN))
		_notMembers[it->first] = it->second;
	_members.erase(it);
	
	if (wasOpe)
		ensureOperator();
}

/* Make sure there is at least one OP in channel*/
void	Channel::ensureOperator(void) {
	MemberMap::iterator	it = _members.begin();

	/* Check if there is another OP in channel */
	for (; it != _members.end(); ++it) {
		if (checkMemberModes(it->first, C_OP))
			return;
	}
	/* If no OP found, assign OP for first member in channel */
	it = _members.begin();
	setMemberModes(it->first, C_OP);
}
