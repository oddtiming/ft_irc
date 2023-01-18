#ifndef JOIN_HPP
#define JOIN_HPP

#pragma once

/* System Includes */
#include <string>
#include <vector>
#include <map>
#include <utility>

/* Local Includes */
#include "Command.hpp"

class Join : public Command
{
    public:
        /* QoL typedefs */
        typedef std::vector< std::pair< std::string, std::string > > ChannelList;
        typedef std::pair<std::string, std::string> StringPair;

        /* Constructors & Destructor */
        Join(Server* server);
        ~Join();

        /* Public Member Functions */
        bool                parse(const Message& msg);
        bool                validate(StringPair channel);
        void                execute(const Message& msg);

        bool                checkInvalidChars(const std::string& string);

    private:
        std::vector< std::pair< std::string, std::string > >    _channels;
        Client*                                                 _client;
};

#endif