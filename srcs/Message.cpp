#include "Message.hpp"
#include "defines.h" // for DEBUG

Message::Message(Client* client, std::string raw) : _client(client)
{
    __SIZE_TYPE__   pos = 0;

    
	/* Optional IRC prefix is denoted by a leading ':' */
    if (raw.at(0) == ':')
    {
        pos = raw.find(' ');
        _prefix = raw.substr(1, pos);
        raw.erase(0, pos + 1);
    }
	
	/* Separate the trailing to keep the entire string as a single parameter */
    //FIXME: When command is "QUIT :" this removes colon causing segfault below
    //This is affecting this entire function, and propogates further as QUIT is not sent as the command name going onwards
    if ((pos = raw.find(" :")) != std::string::npos)
    {
        _trailing = raw.substr(pos + 2, raw.size());
        raw = raw.substr(0, pos + 2);
    }


	/* Split the rest of the parameters */
    while ((pos = raw.find(' ')) != std::string::npos || (pos = raw.find("\r\n")) != std::string::npos)
    {
        _middle.push_back(raw.substr(0, pos));
        raw.erase(0, pos + 1);
    }
	
	/* Transform the command to lowercase, to treat it internally */
    if (getMiddle().empty() == false)
	    _cmd = getMiddle().at(0);
    else //temporary fix for QUIT, not sure how interacts with other commands
        _cmd = raw;
	std::transform(_cmd.begin(), _cmd.end(), _cmd.begin(), ::tolower);

	/* Remove the command from the parameters */
    if (getMiddle().empty() == false)
	    _middle.erase(_middle.begin());

    //  if (DEBUG)
    //  {
    //      std::cerr << "Message received:" << "\n"
	// 	 			<< "_prefix :" << _prefix << "\n"
    //                << "_cmd: " << _cmd << "\n"
    //                << "_trailing: " << _trailing << "\n"
    //                << "_middle.size(): " << _middle.size() << std::endl;
    //      for (size_t i = 0; i < _middle.size(); ++i)
    //         std::cerr << "_middle[" << i << "]: " << _middle.at(i) << std::endl;
    //  }
}
