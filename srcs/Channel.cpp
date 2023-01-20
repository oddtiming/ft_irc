/* Local Includes */
#include "Channel.hpp"
#include "Client.hpp"
#include "defines.h"

class Client;


/* Constructors & Destructor */
Channel::Channel(const std::string& name, Client* owner) : _name(name), _owner(owner) {

	/* Set default channel modes */
	setModes(TOPIC_SET_OP | NO_MSG_IN);

	/* Set channel password */
	//FIXME: Need to set +k flag is password provided. Might want to change so password can only be assigned after channel creation
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
			return (false);
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
void	Channel::addMember(Client* client, const std::string& reply, int modes) {
	/* Add member and set default member modes */
	_members.insert(std::pair<Client*, int>(client, modes));
	sendToAll(reply);
	
	if (DEBUG)
		std::cout << GREEN "New member: " CLEAR << client->getNickname() << GREEN " joined channel: " CLEAR << this->getName() << std::endl;
}

/* Remove a member from channel */
void	Channel::removeMember(Client* client) {
	bool				wasOpe = false;
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
	//FIXME: Ensure that deleting from memberModes right after will not remove this

	/* Erase member from channel */
	_members.erase(it);
	
	/* If member was operator, make sure there is at least one operator in channel */
	if (wasOpe)
		ensureOperator();
}

std::string Channel::getMemberList(void) {
	MemberMap::iterator it = _members.begin();
	std::string list;

	//fixme either do vector+sort to have owner+ops at the top of list or sort them that way while adding them
	/*loops through the MemberMap and build a space-separated list of every nickname
	 * on this channel, adding a '@' in front of the nick of owner/ops */
	for (; it != _members.end(); ++it) {
		if (it->second == C_OP || it->second == OWNER)
			list.append("@" + it->first->getNickname());
		else
			list.append(it->first->getNickname());
		list.append(" ");
	}
	return list;
}

std::vector<std::string> Channel::getMemberVector(void) {
	MemberMap::iterator it = _members.begin();
	std::vector<std::string> list;

	//fixme either do vector+sort to have owner+ops at the top of list or sort them that way while adding them
	/*loops through the MemberMap and build a space-separated list of every nickname
	 * on this channel, adding a '@' in front of the nick of owner/ops */
	for (; it != _members.end(); ++it) {
		if (it->second == C_OP || it->second == OWNER)
			list.push_back("@" + it->first->getNickname());
		else
			list.push_back(it->first->getNickname());
	}
	return list;
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

/* Send message to all members of channel */
void	Channel::sendToOthers(const std::string& reply, Client* sender) {
	MemberMap::iterator it = _members.begin();

	for (; it != _members.end(); ++it)
	{
		if (it->first != sender)
			it->first->reply(reply);
	}
}

void	Channel::sendToAll(const std::string& reply) {
	MemberMap::iterator it = _members.begin();
	for (; it != _members.end(); ++it)
			it->first->reply(reply);
}
