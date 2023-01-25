#include "Message.hpp"
#include "defines.h"
#include <iostream>

Message::Message(Client* client, std::string raw) : _client(client)
{
    size_t   pos    = 0;
    _hasPrefix      = false;
    _hasTrailing    = false;
    _hasMiddle      = false;
    
	/* Seperate prefix from message */
    if (raw.at(0) == ':')
    {
        pos = raw.find(' ');
        _prefix = raw.substr(1, pos);
        raw.erase(0, pos + 1);
        _hasPrefix = true;
    }

    /* Seperate trailing string from message */
    if ((pos = raw.find(" :")) != std::string::npos)
    {
        _trailing = raw.substr(pos + 2, raw.size());
        raw = raw.substr(0, pos + 2);

        if ((pos = _trailing.find("\r\n")) != std::string::npos)
            _trailing = _trailing.substr(0, pos);
        _hasTrailing = true;
    }

	/* Seperate the middle parameters into a vector of strings */
    while ((pos = raw.find(' ')) != std::string::npos || (pos = raw.find("\r\n")) != std::string::npos)
    {
        _middle.push_back(raw.substr(0, pos));
        raw.erase(0, pos + 1);
    }
	
    /* Remove command from middle, and transform to lower case for key matching with _commands map */
    if (getMiddle().empty() == false)
    {
	    _cmd = _middle.at(0);
        _middle.erase(_middle.begin());
	    std::transform(_cmd.begin(), _cmd.end(), _cmd.begin(), ::tolower);
    }

    /* Check if any other parameters remain in middle */
    if (!_middle.empty())
        _hasMiddle = true;

    

    /* DEBUG PRINTOUTS */
    // std::cerr << "Prefix: " << _prefix << std::endl;
    // std::cerr << "Command: " << _cmd << std::endl;
    // std::cerr << "Parameters: " << std::endl;

    // std::vector<std::string>::iterator it = _middle.begin();
    // for (; it != _middle.end(); ++it)
    //     std::cerr << *it << std::endl;
    
    // std::cerr << "Trailing: " << _trailing << std::endl;

}
