Quit::Quit(Server* server) : Command("quit", server) {
	_channelOpRequired = false;
	_globalOpRequired = false;
}

Quit::~Quit() {

}

bool	Quit::validate(const Message& msg) {

}

void	Quit::execute(const Message& msg) {
	if (validate(msg))
	{
		/* Check permissions for execution of function */
		std::string	message = "QUIT :";
		if (middle.size() > 1)
			message += middle.at(0);
		msg._client->reply(message);
		//TODO: add code to remove client from irc and all things associated
	}
}