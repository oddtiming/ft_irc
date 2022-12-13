#include "Message.hpp"
Message::~Message() { }

// TODO: if "\r\n" is present, store it in _store
Message::Message(Client* client, std::string raw) : _client(client)
{
    __SIZE_TYPE__   pos = 0;

    size_t   it = raw.find("\r\n");
    if (it != raw.size() - 2)
    {
        _store = raw.substr(it + 2, raw.size() - it - 2);
        raw = raw.s
    }
    if (raw[0] == ':')
    {
        pos = raw.find(' ');
        _prefix = raw.substr(1, pos);
        raw.erase(0, pos + 1);
    }
    while ((pos = raw.find(' ')) != std::string::npos)
    {
        _params.push_back(raw.substr(0, pos));
        raw.erase(0, pos + 1);
    }
	_cmd = getParams()[0];
	_params.erase(_params.begin());
}



/* Setters & Getters */
const	std::string&	Message::getPrefix() const {
    return (_prefix);
}

const 	std::string&	Message::getCommand() const{
    return(_cmd);
}

const	std::vector<std::string>&	Message::getParams() const {
    return (_params);
}

const Client*	Message::getClient() const {
	return (_client);
}		

/* Public Member Functions */
