#ifndef TOPIC_HPP
#define TOPIC_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "../Command.hpp"

class Topic : public Command
{
	public:
		/* Constructors & Destructor */
		Topic(Server *server);
		~Topic() { }

		/* Public Member Functions */
		bool                validate(const Message& msg);
		void                execute(const Message& msg);
		void				clearData();

	private:
		Client*				_client;
		Channel*			_channel;
		std::string 		_target;
		std::string			_topic;
};

#endif