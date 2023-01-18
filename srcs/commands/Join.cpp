#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join(Server* server) : Command("join", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */

/* Parse raw message data into vector of channel/password pairs */
bool	Join::parse(const Message& msg) {
	/* Error message if not enough information received to execute command */
	if (msg.getMiddle().empty())
	{
		_client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return (false);
	}

	/* Split all requested channels into _channels vector */
	std::string rawChannels = msg.getMiddle().at(0);
	size_t posChannels = rawChannels.find(',');
	std::string rawPasswords;
	size_t posPasswords = std::string::npos;
	// std::string empty;
	bool pass = false;
	
	/* Check passwords were also provided */
	if (msg.getMiddle().size() > 1)
	{
		pass = true;
		rawPasswords = msg.getMiddle().at(1);
		posPasswords = rawPasswords.find(',');
	}

	/* Iterate through channels and add channel/password pair to vector (_channels) */
	while ((posChannels = rawChannels.find(',')) != std::string::npos)
	{
		/* If passwords and still remaining passwords add name/pass to vector */
		if (pass && (posPasswords != std::string::npos))
		{
			_channels.push_back(StringPair(rawChannels.substr(0, posChannels), rawPasswords.substr(0, posPasswords)));
			rawPasswords.erase(0, posPasswords + 1);
		}
		/* Otherwise add name and empty password to vector */
		else
			_channels.push_back(StringPair(rawChannels.substr(0, posChannels), std::string()));
		rawChannels.erase(0, posChannels + 1);
	}
	/* Add remaining name/pass to vector */
	if (pass && (posPasswords != std::string::npos))
		_channels.push_back(StringPair(rawChannels, rawPasswords));
	else
		_channels.push_back(StringPair(rawChannels, std::string()));
	return (true);
}

/* Validate channel join requests before attempting to exectute*/
bool	Join::validate(StringPair channel) {
	
	/* QoL variables*/
	std::string name = channel.first;
	std::string pass = channel.second;

	/* Check if first character is valid */
	if (name.size() > 0 && name.at(0) != '#')
	{
		_client->reply(ERR_BADCHANMASK(name));
		return (false);
	}

	/* Check if name contains non-printable characters or is longer than 200 chars */
	if (name.size() > 200 || !checkInvalidChars(name))
	{
		_client->reply(ERR_NOSUCHCHANNEL(name));
		return (false);
	}

	/* If channel does exist, check modes and password */
	if (_server->doesChannelExist(name))
	{
		/* QoL variable */
		Channel* channelPtr = _server->getChannelPtr(name);

		/* If user is already a member of channel then do nothing */
		if (channelPtr->isMember(_client))
			return (false);

		/* Check if channel is invite only & user is not invited */
		if (channelPtr->checkModes(INV_ONLY) && !channelPtr->checkMemberModes(_client, INV))
		{
			_client->reply(ERR_INVITEONLYCHAN(name));
			return (false);
		}

		/* Check if client is banned from channel */
		if (channelPtr->checkMemberModes(_client, BAN))
		{
			_client->reply(ERR_BANNEDFROMCHAN(name));
			return (false);
		}

		/* If channel is password protected check password match*/
		if (channelPtr->checkModes(PASS_REQ) && channelPtr->getPass() != pass)
		{
			_client->reply(ERR_BADCHANNELKEY(name));
			return (false);
		}

	}
	return (true);
}
	//FIXME: Implement +k for channel password mode

void	Join::execute(const Message& msg) {

	/* QoL variable init */
	_client = msg._client;

	/* Parse raw message into vector of channel/password pairs */
	if (!parse(msg))
		return;
	
	/* Iterate through channels list and attempt to validate and exectute for each one */
	ChannelList::iterator 	ite = _channels.end();
	bool					hasJoined = true;
	
	for (ChannelList::iterator it = _channels.begin(); it != ite; ++it)
	{
		/* QoL variables */
		std::string name = it->first;
		std::string pass = it->second;
		Channel* channelPtr;

		/* Attempt to validate*/
		if (validate(*it) == false)
			continue;

		/* If channel does not exist, create it */
		if (!_server->doesChannelExist(name))
		{
			_server->createChannel(name, _client);
			hasJoined = false;
		}
		/* Otherwise attempt to join channel */
		channelPtr = _server->getChannelPtr(name);
		channelPtr->addMember(_client, CMD_JOIN(_buildPrefix(msg), name));

		/* Send reply messages */
		_client->reply(RPL_NAMREPLY(_server->getHostname(), _client->getNickname(), name, channelPtr->getMemberList()));
		_client->reply(RPL_ENDOFNAMES(_server->getHostname(), _client->getNickname(), name));

		/* Manage topic reply*/
		if (hasJoined && channelPtr->getTopic().size() > 0)
			_client->reply(RPL_TOPIC(name, channelPtr->getTopic()));
		else if (hasJoined)
			_client->reply(RPL_NOTOPIC(name));
	}		
}

/* Check for unprintable characters in channel name */
bool	Join::checkInvalidChars(const std::string& string) {
	std::string::const_iterator it = string.begin();

	for (; it != string.end(); ++it)
	{
		if (!isprint(*it))
			return (false);
	}
	return (true);
}

