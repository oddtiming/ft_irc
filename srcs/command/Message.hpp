class Command{
private:
	User *user;
	Channel *channel;
	std::string message;
	std::string prefix;
	std::vector<std::string> params;

public:
	Command(User *user, Channel *channel, std::string message);
	std::string getReplies(unsigned int code);
	std::string getPrefix();
	std::string	getParams();
};