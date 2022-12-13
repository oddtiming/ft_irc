#include "Message.hpp"
Message::~Message() { }

Message::Message(User* user, std::string raw) : _user(user)
{
    __SIZE_TYPE__   pos = 0;

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

const	std::string&	Message::getPrefix() const {
    return (_prefix);
}

const 	std::string&	Message::getCommand() const{
    return(_cmd);
}

const	std::vector<std::string>&	Message::getParams() const {
    return (_params);
}
