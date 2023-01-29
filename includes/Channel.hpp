#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

/* System Includes */
#include <ctime>
#include <iostream>
#include <map>
#include <stdint.h> // uint32_t
#include <string>
#include <vector>

/* Funtion Prototype */
const std::string getTimestamp( );

/* Class Definitions */
class Client;

class Channel
{
  public:
	/* Members are stored with their addresses as the key, and their memberModes as the
	 * value */
	typedef uint32_t                  Mode;
	typedef std::map< Client*, Mode > MemberMap;

	Channel(const std::string& name, Client* owner);
	~Channel( ) {}

	void closeChannel( );

	/*************************/
	/*   Setters & Getters   */
	/*************************/
	const std::string& getName( ) { return (_name); }
	const std::string& getTopic( ) { return (_topic); }
	void               setTopic(const std::string topic) { _topic = topic; }
	void               setOwner(Client* owner) { _owner = owner; }
	void               setPass(const std::string& pass) { _password = pass; }
	Client*            getOwner(void) const { return (_owner); }
	std::string        getPassword( ) { return (_password); }
	const std::time_t& getStartTime(void) const { return _timeStart; }
	int                getHighestRank(Client* client);

	/*******************************/
	/*        Mode Management      */
	/*******************************/
	void        setModes(char modes, bool removeMode = false);
	void        setMemberModes(Client* client, char modes, bool removeMode = false);
	void        setPassword(const std::string& password) { _password = password; }
	bool        checkModes(char modes) const { return (_modes & modes) == modes; }
	bool        checkMemberModes(Client* client, char modes) const;
	std::string getChannelModes(void) const;
	std::vector< std::string > getBanList(void) const;

	/***********************************/
	/*    Channel Member Management    */
	/***********************************/
	bool        isMember(Client* client);
	void        addMember(Client* client, const std::string& reply, int modes = 0);
	void        removeMember(Client* client);
	void        ensureOperator(void);
	std::string getMemberList(void);
	size_t      getNbVisibleUsers(void) const;
	bool        getIsEmpty( ) const { return _members.empty( ); }
	std::vector< std::string > getMemberVector(void);

	/* Channel Messaging */

	void sendToAll(const std::string& reply);
	void sendToOthers(const std::string& reply, Client* sender);

  private:
	const std::string _name;     /* Channel name */
	std::string       _password; /* if channel is password protected */
	std::string       _topic;    /* Channel topic */
	Client*           _owner;    /* Channel owner */
	MemberMap         _members;  /* Channel member list, paired with their memberModes */
	MemberMap
	     _notMembers; /* Clients with flags set who are not currently in the channel */
	char _modes;      /* Channel modes */
	const std::time_t _timeStart; /* Time channel was created */
};

#endif