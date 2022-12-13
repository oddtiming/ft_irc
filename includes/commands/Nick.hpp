#ifndef NICK_HPP
#define NICK_HPP

#include "../Command.hpp"

class Nick : public Command{
	public:
		Nick();
		const std::string&	execute(const Message& msg);
	 	bool				validate(const Message& msg);

};