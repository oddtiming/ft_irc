
/* Local Includes */
#include "Channel.hpp"
#include "defines.h"


/* Constructors & Destructor */
Channel::Channel(const std::string& name, const std::string& pass, Client* owner) : _name(name), _owner(owner) {

	/* Set default channel modes */
	setModes(TOPIC_SET_OP | NO_MSG_IN, true);

	/* Add channel creator to member list */
	_members[owner->getNickname()] = owner;

	/* Assign member modes to channel creator */
	setMemberModes(owner->getNickname(), OWNER, true);
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
void	Channel::setModes(char modes, bool isAdd) {
	if (isAdd) {
		_modes = (_modes | modes);
	}
	else {
		_modes = (_modes ^ modes);
		//FIXME: Ensure use of XOR is correct to remove flags from mask
	}
}

/* Set member mode flags for a specified client */
void	Channel::setMemberModes(const std::string& nick, char modes, bool isAdd) {
	std::map<std::string, char>::iterator it = _memberModes.find(nick);
	/* Return without doing anything if user not found in list */
	if (it == _memberModes.end())
		return;
	if (isAdd) {
		it->second = (it->second | modes);
	}
	else {
		it->second = (it->second ^ modes);
		//FIXME: Ensure use of XOR is correct to remove flags from mask
	}
}

/* Check channel wide mode flags */
bool	Channel::checkModes(char modes){
	return (_modes == (_modes & modes));
}

/* Check member mode flags for specified client */
bool	Channel::checkMemberModes(const std::string& nick, char modes) {
	std::map<std::string, char>::iterator it = _memberModes.find(nick);
	/* Return without doing anything if user not found in list */
	if (it == _memberModes.end())
		return;
	return (it->second == (it->second & modes));
}


/***********************************/
/*    Channel Member Management    */
/***********************************/

/* Check if specified client is a member of channel */
bool	Channel::isMember(const std::string& nick) {
	std::map<std::string, Client*>::iterator it = _members.find(nick);
	if (it == _members.end())
		return(false);
	return (true); 
}

/* Add a new member to channel */
void	Channel::addMember(Client* client) { 
	_members[client->getNickname()] = client;
}

/* Remove a member from channel */
void	Channel::removeMember(const std::string& nick) {
	std::map<std::string, Client*>::iterator it = _members.find(nick);
	if (it == _members.end())
		return;

	/* Check if member is owner */
	// if (it->second->getNickname() == _owner->getNickname())
		//Set new owner
	//FIXME: Decide how to choose new owner
	/* Remove member */
	_members.erase(it);
	/* Remove member modes */
	std::map<std::string, char>::iterator modeIt = _memberModes.find(nick);
	_memberModes.erase(modeIt);
}

