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
	/* Assign client to a pointer to clean up code */
	Client* client = msg._client;

	/* Error message if not enough information received to execute command */
	if (msg.getMiddle().empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return (false);
	}
	/* Split all requested channels into _channels vector */
	else
	{
		std::string rawChannels = msg.getMiddle().at(0);
		size_t posChannels = rawChannels.find(',');
		std::string rawPasswords;
		size_t posPasswords = std::string::npos;
		std::string empty;
		bool pass = false;
		/* If passwords were also provided */
		if (msg.getMiddle().size() > 1)
		{
			pass = true;
			rawPasswords = msg.getMiddle().at(1);
			posPasswords = rawPasswords.find(',');
		}

		/* Iterate through channels and add to vector (_channels) */
		while ((posChannels = rawChannels.find(',')) != std::string::npos)
		{
			/* If passwords and still remaining passwords add name/pass to vector */
			if (pass && (posPasswords != std::string::npos))
			{
				_channels.push_back(std::pair<std::string, std::string>(rawChannels.substr(0, posChannels), rawPasswords.substr(0, posPasswords)));
				rawPasswords.erase(0, posPasswords + 1);
			}
			/* Otherwise add name and empty password to vector */
			else
				_channels.push_back(std::pair<std::string, std::string>(rawChannels.substr(0, posChannels), empty));
			rawChannels.erase(0, posChannels + 1);
		}
		/* Add remaining name/pass to vector */
		if (pass && (posPasswords != std::string::npos))
			_channels.push_back(std::pair<std::string, std::string>(rawChannels, rawPasswords));
		else
			_channels.push_back(std::pair<std::string, std::string>(rawChannels, empty));
	}

	// /* DEBUG PRINTING */
	// std::vector<std::pair<std::string, std::string> >::iterator it = _channels.begin();
	// for (; it != _channels.end(); ++it)
	// {
	// 	std::cout << "Channel: " << it->first << " - Password: " << it->second << std::endl;
	// }

	/* Iterate through channels and check each one */
	std::vector<std::pair<std::string, std::string> >::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
	{
		/* Assign channel name and password to string to clean up code */
		std::string name = it->first;
		std::string pass = it->second;
		/* Check if first character is valid */
		if (name.size() > 0 && name.at(0) != '#')
			client->reply(ERR_BADCHANMASK(name));
		/* Check if name contains non-printable characters or is longer than 200 chars*/
		else if (name.size() > 200 || !checkInvalidChars(name))
			client->reply(ERR_NOSUCHCHANNEL(name));
		/* If channel does not exist, then create it*/
		else if (!_server->doesChannelExist(name))
		{
				_server->createChannel(name, client);
				/* Send reply messages */
				client->reply(":jgoad!jgoad@127.0.0.1 JOIN :#new\r\n");
				client->reply(":localhost 353 jgoad = #new :@jgoad\r\n");
				client->reply(":localhost 366 jgoad #new :End of /NAMES list.\r\n");

				// client->reply(CMD_JOIN(_buildPrefix(msg), name));
				// client->reply(":ircserv 353 jgoad = #new1 :@jgoad\r\n");
				// client->reply(RPL_ENDOFNAMES(_server->getHostname(), client->getNickname(), name));
				if (DEBUG)
					std::cout << BLUE"New channel created: " CLEAR << name << std::endl;
		/*
		Tue Jan 17 2023 12:51:03 USERINPUT: C[422AAAAAA] I JOIN #new
		Tue Jan 17 2023 12:51:03 USEROUTPUT: C[422AAAAAA] O :jgoad!jgoad@127.0.0.1 JOIN :#new
		Tue Jan 17 2023 12:51:03 USEROUTPUT: C[422AAAAAA] O :penguin.omega.example.org 353 jgoad = #new :@jgoad
		Tue Jan 17 2023 12:51:03 USEROUTPUT: C[422AAAAAA] O :penguin.omega.example.org 366 jgoad #new :End of /NAMES list.
		Tue Jan 17 2023 12:51:03 USERINPUT: C[422AAAAAA] I MODE #new +sn
		Tue Jan 17 2023 12:51:03 USEROUTPUT: C[422AAAAAA] O :jgoad!jgoad@127.0.0.1 MODE #new :+s
		*/


		}
		/* If channel does exist attempt to join */
		else
		{
			/* Get channel pointer to clean up code */
			Channel* channel = _server->getChannelPtr(name);
			/* If user is already a member of channel then do nothing */
			if (channel->isMember(client))
				continue;
			/* Check if channel is invite only & user is not invited */
			else if (channel->checkModes(INV_ONLY) && !channel->checkMemberModes(client, INV))
				client->reply(ERR_INVITEONLYCHAN(name));
			/* Check if client is banned from channel */
			else if (channel->checkMemberModes(client, BAN))
				client->reply(ERR_BANNEDFROMCHAN(name));
			else
			{
				/* If channel is password protected check password match*/
				if (channel->checkModes(PASS_REQ) && channel->getPass() != pass)
					client->reply(ERR_BADCHANNELKEY(name));
				/* Join channel */
				else
				{
					channel->addMember(client);
					/* Send reply messages */
					client->reply(CMD_JOIN(_buildPrefix(msg), name));
					//TMP namereply to test
					client->reply(":ircserv 353 jgoad = #new1 :@jgoad\r\n");
					// :penguin.omega.example.org 353 Jon__ = #new :@Jon Jon_ Jon__
					// msg._client->reply(RPL_NAMREPLY());
					client->reply(RPL_ENDOFNAMES(_server->getHostname(), msg._client->getNickname(), name));
					if (channel->getTopic().size() > 0)
						client->reply(RPL_TOPIC(name, channel->getTopic()));
					else
						client->reply(RPL_NOTOPIC(name));
				}
			}
		}
	}
	return (true);
	//FIXME: Not sure if we want to implement max channels per client
	//FIXME: Not sure if we want to implement max members per channel
	//FIXME: Need to check /names response on join
	//FIXME: Implement +k for channel password mode
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
/*
NICK jgoad
USER jgoad 0 * jgoad
:penguin.omega.example.org 001 jgoad :Welcome to the Omega IRC Network jgoad!jgoad@127.0.0.1
*/