#include "commands/Join.hpp"

/* Constructors & Destructor */
Join::Join(Server* server) : Command("join", server) {
	this->_channelOpRequired = false;
	this->_globalOpRequired = false;
}

Join::~Join() {
}

/* Public Member Functions */
bool	Join::validate(const Message& msg) {
	/* Error message if not enough information received to execute command */
	if (msg.getMiddle().empty())
	{
		msg._client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return (false);
	}
	else
	{
		/* Split all requested channels into _channels vector */
		std::string rawChannels = msg.getMiddle().at(0);
		size_t posChannels = rawChannels.find(',');
		while ((posChannels = rawChannels.find(',')) != std::string::npos)
		{
			_channels.push_back(rawChannels.substr(0, posChannels));
			rawChannels.erase(0, posChannels + 1);
		}
		_channels.push_back(rawChannels);

		/* If more than one parameter, split all passwords into _passwords vector */
		if (msg.getMiddle().size() > 1)
		{
			std::string rawPasswords = msg.getMiddle().at(1);
			size_t posPasswords = rawPasswords.find(',');
			while ((posPasswords = rawPasswords.find(',')) != std::string::npos)
			{
				_passwords.push_back(rawPasswords.substr(0, posPasswords));
				rawPasswords.erase(0, posPasswords + 1);
			}
			_passwords.push_back(rawPasswords);
		}
	}
	//FIXME: Not sure if we want to implement max channels per client
	//FIXME: Not sure if we want to implement max members per channel
	//FIXME: Need to check /names response on join
	//FIXME: Implement +k for channel password mode


	/* Iterate through channels and check each one */
	std::vector<std::string>::iterator itChannels = _channels.begin();
	std::vector<std::string>::iterator itPasswords = _passwords.begin();

	

	for (; itChannels != _channels.end(); ++ itChannels)
	{
		/* Check if first character is valid */
		if (itChannels->size() > 0 && itChannels->at(0) != '#')
			msg._client->reply(ERR_BADCHANMASK(*itChannels));
		/* Check if name contains non-printable characters or is longer than 200 chars*/
		else if (itChannels->size() > 200 || !checkInvalidChars(*itChannels))
			msg._client->reply(ERR_NOSUCHCHANNEL(*itChannels));
		/* If channel does not exist, then create it*/
		else if (!_server->doesChannelExist(*itChannels))
		{
				_server->createChannel(*itChannels,msg._client);
				std::cout << BLUE"Channel Created" CLEAR << std::endl;
		}
		/* If channel does exist attempt to join */
		else
		{
			Channel* channel = _server->getChannelPtr(*itChannels);
			/* If user is already a member of channel then do nothing */
			if (channel->isMember(msg._client))
			{
				if (itPasswords != _passwords.end())
					itPasswords++;
				continue;
			}
			/* Check if channel is invite only & user is not invited */
			else if (channel->checkModes(INV_ONLY) && !channel->checkMemberModes(msg._client, INV))
				msg._client->reply(ERR_INVITEONLYCHAN(*itChannels));
			/* Check if client is banned from channel */
			else if (channel->checkMemberModes(msg._client, BAN))
				msg._client->reply(ERR_BANNEDFROMCHAN(*itChannels));
			else
			{
				/* If channel is password protected */
				if (channel->checkModes(PASS_REQ) && (itPasswords == _passwords.end() || (itPasswords != _passwords.end()  && channel->getPass() != *itPasswords)))
					msg._client->reply(ERR_BADCHANNELKEY(*itChannels));
				else
				{
					channel->addMember(msg._client);
					msg._client->reply(CMD_JOIN(_buildPrefix(msg), *itChannels));
					if (channel->getTopic().size() > 0)
						msg._client->reply(RPL_TOPIC(*itChannels, channel->getTopic()));
					else
						msg._client->reply(RPL_NOTOPIC(*itChannels));
				}
			}
		}
		if (itPasswords != _passwords.end())
			itPasswords++;
		
	}

	
		
	return (true);
}

void	Join::execute(const Message& msg) {

	if (validate(msg))
	{
		return;
		/* Iterate through channels and passwords */

		/* Check if channel exists and */
		
		/* If channel doesn't exist, set create and set user as admin */


	}
}

bool	Join::checkInvalidChars(const std::string& string) {
	std::string::const_iterator it = string.begin();

	for (; it != string.end(); ++it)
	{
		if (!isprint(*it))
			return (false);
	}
	return (true);
}