#ifndef MODE_HPP
#define MODE_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Mode : public Command
{
    public:
        /* Constructors & Destructor */
        Mode(Server* server);
        ~Mode();

        /* Public Member Functions */


		/* Function Overloads for Validate */

        bool                validateTarget(void);           /* Check if target channel or user exists */
        bool                executeMode(char mode, bool removeMode);           /* Dispatches the mode to the correct command */
        void                execute(const Message& msg);
        bool                parse(const Message& msg);
        void                clearAttributes(void);

		void				sendChannelModes();
		void				sendUserModes();
		void				sendBanList() const;

    private:
        Client*				_client;
        /* Optional field for cases where the target is a channel, and a user is
         * specified. (e.g. "/mode #[channel] +[modes] [nick]")
         */
        std::vector<std::string>	_params;	// Represents potential parameters required by specific modes (+k, +b)
		std::string					_target;
        int         				_targetType;
        std::string         		_modes;
        std::string					_reply;

		/* Individual mode handlers */
        bool                _secret(bool removeMode);
        bool                _topic(bool removeMode);
        bool                _invite(bool removeMode);
        bool                _no_msg_in(bool removeMode);
        bool                _password(bool removeMode);
        bool                _ban(bool removeMode);
        bool                _operators(bool removeMode);
};

#endif