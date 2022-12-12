#include "replies.hpp"

std::string Command::getReplies(ARGS)
{
	switch(reply) {
		case RPL_WELCOME:
			return "Welcome to the Internet Relay Network <nick>!<user>@<host>"
		case RPL_YOURHOST:
			return "Your host is <servername>, running version <ver>"
		case RPL_CREATED:
			return  "This server was created <date>"
		case RPL_MYINFO:
			return "<servername> <version> <available user modes <available channel modes>"
		case RPL_BOUNCE:
			return "Try server <server name>, port <port number>"
		case RPL_TRACELINK:
			return "Link <version & debug level> <destination> <next server> V<protocol version><link uptime in seconds> <backstream sendq><upstream sendq>"
		case RPL_TRACECONNECTING:
			return "Try. <class> <server>"
		case RPL_TRACEHANDSHAKE:
			return "H.S. <class> <server>"
		case RPL_TRACEUNKNOWN:
			return "???? <class> [<client IP address in dot form>]"
		case RPL_TRACEOPERATOR:
			return "Oper <class> <nick>"
		case RPL_TRACEUSER:
			return "User <class> <nick>"
		case RPL_TRACESERVER:
			return "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server> V<protocol version>"
		case RPL_TRACENEWTYPE:
			return "<newtype> 0 <client name>"
		case RPL_STATSLINKINFO:
			return "<linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>"
		case RPL_STATSCOMMANDS:
			return "<command> <count> <byte count> <remote count>"
		case RPL_ENDOFSTATS:
			return "<stats letter> :End of STATS report"
		case RPL_UMODEIS:
			return "<user mode string>"
		case RPL_STATSUPTIME:
			return ":Server Up %d days %d:%02d:%02d"
		case RPL_STATSOLINE:
			return "O <hostmask> * <name>"
		case RPL_LUSERCLIENT:
			return ":There are <integer> users and <integer> services on <integer> servers"
		case RPL_LUSEROP:
			return  "<integer> :operator(s) online"
		case RPL_LUSERUNKNOWN:
			return  "<integer> :unknown connection(s)"
		case RPL_LUSERCHANNELS:
			return  "<integer> :channels formed"
		case RPL_LUSERME:
			return  ":I have <integer> clients and <integer> servers"
		case RPL_ADMINME:
			return "<server> :Administrative info"
		case RPL_ADMINLOC1:
			return ":<admin info>"
		case RPL_ADMINLOC2:
			return ":<admin info>"
		case RPL_ADMINEMAIL:
			return  ":<admin info>"
		case RPL_TRACELOG:
			return  "File <logfile> <debug level>"
		case RPL_TRACEEND:
			return  "<server name> <version & debug level> :End of TRACE"
		case RPL_TRYAGAIN:
			return  "<command> :Please wait a while and try again."
		case RPL_AWAY:
			return
		case RPL_USERHOST:
			return
		case RPL_ISON:
			return
		case RPL_UNAWAY:
			return
		case RPL_NOWAWAY:
			return
		case RPL_WHOISUSER:
			return
		case RPL_WHOISSERVER:
			return
		case RPL_WHOISOPERATOR:
			return
		case RPL_WHOWASUSER:
			return
		case RPL_ENDOFWHO:
			return
		case RPL_WHOISIDLE:
			return
		case RPL_ENDOFWHOIS:
			return
		case RPL_WHOISCHANNELS:
			return
		case RPL_LISTSTART:
			return
		case RPL_LIST:
			return
		case RPL_LISTEND:
			return
		case RPL_CHANNELMODEIS:
			return
		case RPL_UNIQOPIS:
			return
		case RPL_NOTOPIC:
			return
		case RPL_TOPIC:
			return
		case RPL_INVITING:
			return
		case RPL_SUMMONING:
			return
		case RPL_INVITELIST:
			return
		case RPL_ENDOFINVITELIST:
			return
		case RPL_EXCEPTLIST:
			return
		case RPL_ENDOFEXCEPTLIST:
			return
		case RPL_VERSION:
			return
		case RPL_WHOREPLY:
			return
		case RPL_NAMREPLY:
			return
		case RPL_LINKS:
			return
		case RPL_ENDOFLINKS:
			return
		case RPL_ENDOFNAMES:
			return
		case RPL_BANLIST:
			return
		case RPL_ENDOFBANLIST:
			return
		case RPL_ENDOFWHOWAS:
			return
		case RPL_INFO:
			return
		case RPL_MOTD:
			return
		case RPL_ENDOFINFO:
			return
		case RPL_MOTDSTART:
			return
		case RPL_ENDOFMOTD:
			return
		case RPL_YOUREOPER:
			return
		case RPL_REHASHING:
			return
		case RPL_YOURESERVICE:
			return
		case RPL_TIME:
			return
		case RPL_USERSSTART:
			return
		case RPL_USERS:
			return
		case RPL_ENDOFUSERS:
			return
		case RPL_NOUSERS:
			return
		case ERR_NOSUCHSERVER:
			return
		case ERR_NOSUCHCHANNEL:
			return
		case ERR_CANNOTSENDTOCHAN:
			return
		case ERR_TOOMANYCHANNELS:
			return
		case ERR_WASNOSUCHNICK:
			return
		case ERR_TOOMANYTARGETS:
			return
		case ERR_NOSUCHSERVICE:
			return
		case ERR_NOORIGIN:
			return
		case ERR_NORECIPIENT:
			return
		case ERR_NOTEXTTOSEND:
			return
		case ERR_NOTOPLEVEL:
			return
		case ERR_WILDTOPLEVEL:
			return
		case ERR_BADMASK:
			return
		case ERR_UNKNOWNCOMMAND:
			return
		case ERR_NOMOTD:
			return
		case ERR_NOADMININFO:
			return
		case ERR_FILEERROR:
			return
		case ERR_NONICKNAMEGIVEN:
			return
		case ERR_ERRONEUSNICKNAME:
			return
		case ERR_NICKNAMEINUSE:
			return
		case ERR_NICKCOLLISION:
			return
		case ERR_USERNOTINCHANNEL:
			return
		case ERR_NOTONCHANNEL:
			return
		case ERR_USERONCHANNEL:
			return
		case ERR_NOLOGIN:
			return
		case ERR_SUMMONDISABLED:
			return
		case ERR_USERSDISABLED:
			return
		case ERR_NOTREGISTERED:
			return
		case ERR_NEEDMOREPARAMS:
			return
		case ERR_ALREADYREGISTRED:
			return
		case ERR_NOPERMFORHOST:
			return
		case ERR_PASSWDMISMATCH:
			return
		case ERR_YOUREBANNEDCREEP:
			return
		case ERR_KEYSET:
			return
		case ERR_CHANNELISFULL:
			return
		case ERR_UNKNOWNMODE:
			return
		case ERR_INVITEONLYCHAN:
			return
		case ERR_BANNEDFROMCHAN:
			return
		case ERR_BADCHANNELKEY:
			return
		case ERR_BADCHANMASK:
			return
		case ERR_NOCHANMODES:
			return
		case ERR_BANLISTFULL:
			return
		case ERR_NOPRIVILEGES:
			return
		case ERR_CHANOPRIVSNEEDED:
			return
		case ERR_CANTKILLSERVER:
			return
		case ERR_RESTRICTED:
			return
		case ERR_UNIQOPPRIVSNEEDED:
			return
		case ERR_NOOPERHOST:
			return
		case ERR_UMODEUNKNOWNFLAG:
			return
		case ERR_USERSDONTMATCH:
			return

	}
}
