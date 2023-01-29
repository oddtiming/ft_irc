#ifndef WHO_HPP
#define WHO_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Client.hpp"
#include "Command.hpp"
#include "defines.h" // e_targetType{}

class Who : public Command
{
  public:
	/* QoL typedefs */
	typedef std::map< std::string, Channel* > ChannelList;

	/* Constructors & Destructor */
	Who(Server* server);
	~Who( );

	/* Public Member Functions */
	bool validate(const Message& msg);
	void execute(const Message& msg);

  private:
	/* Private member attributes */
	Client* _client;
	void*   _target;
	int     _targetType;

	/* Private member functions */
	void _listChannelMembers();
	void _listClientChannels();
};

#endif