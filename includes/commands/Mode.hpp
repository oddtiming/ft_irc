#ifndef MODE_HPP
#define MODE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "commands/Command.hpp"

class Mode : public Command
{
    public:
        /* Constructors & Destructor */
        Mode(Server* server);
        ~Mode();

        /* Public Member Functions */


		/* Function Overloads for Validate */
		bool				validate(Client* client);
		bool				validate(Channel* channel);

        bool                validate(const Message& msg);
        void                execute(const Message& msg);
        bool                parse(const Message& msg);
        void                clearData(void);

		void				sendChannelModes();
		void				sendUserModes();

    private:
        Client*             _client;
        /* Optional field for cases where the target is a channel, and a user is
         * specified. (e.g. "/mode #[channel] +[modes] [nick]")
         */
        std::string         _targetMember;
		std::string			_target;
        int         		_targetType;
        std::string         _modes;
        std::string			_reply;


		bool 				_targetIsChannel;

};

#endif