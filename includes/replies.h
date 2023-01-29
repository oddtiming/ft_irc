#include "Command.hpp"

/* Reply Messages */
#define RPL_NOWAWAY(host, client) ":" + (host) + " 306 " + (client) + " :You have been marked as being away" + "\r\n" 						//306
#define RPL_WELCOME(host, client, prefix) ":" + (host) +  " 001 " + (client) + " :Welcome to the IRC server " + (prefix) + "\r\n" 			//001
#define RPL_UMODEIS(host, client, modes) ":" + (host) + " 221 " + (client) + " :" + (modes) + "\r\n" 										//221
#define RPL_AWAY(hostname, client, target, message) ":" + (hostname) + " 301 " +  (client) + " " + (target) + " :" + (message) + "\r\n" 	//301
#define RPL_UNAWAY(host, client) ":" + (host) + " 305 " + (client) + " :You are no longer marked as being away" + "\r\n" 					//305
#define RPL_WHOISUSER(host, client, nick, user, address, real) ( ":" + (host) + " 311 " + (client) + " " + (nick) + " " + (user) \
	+ " " + (address) + " * :"+ (real) + "\r\n") 																							//311
#define RPL_WHOISSERVER(host, client, target, servername) (":" + (host) + " 312 " + (client) + " " + (target) + " " + (host) + " :" + \
	(servername) + "\r\n") 																													//312
#define RPL_WHOISIDLE(host, client, target, idle, signon) ( ":" + (host) +  " 317 " + (nick) + " " + (target) + " " + (idle) \
	+ " " + (signon) + " :seconds idle, signon time" + "\r\n")																				//317
#define RPL_ENDOFWHOIS(host, client, target) ":" + (host) + " 318 " + (client) + " " + (target) + " :End of /WHOIS list" + "\r\n" 			//318
#define RPL_WHOISCHANNELS(host, client, target, reply) ( ":" + (host) + " 319 " + (client) + " " + (target) + " :" + \
	(reply) + "\r\n") 																														//319
#define RPL_LISTSTART(host, client) ":" + (host) + " 321 " + (client) + "Channel :Users Name" + "\r\n"										//321
#define RPL_LIST(host, client, channel, nbUsers, topic) ( ":" + (host) + " 322 " + (client) + " " +  (channel) + \
	" " + (nbUsers) + " :" + (topic) +  "\r\n")																							//322
#define RPL_LISTEND(host, nick) ":" + (host) + " 323 " + (nick) + " :End of channel list" + "\r\n" 											//323
#define RPL_CHANNELMODEIS(host, nick, channel, modes) ":" + (host) + " 324 " + (nick) + " " + (channel) + " :" + (modes) + "\r\n" 			//324
#define	RPL_NOTOPIC(host, client, channel) ":" + (host) + " 331 " + (client) + " " + (channel) + " :No topic is set" + "\r\n" 				//331
#define	RPL_TOPIC(host, client, channel, topic) ":" + (host) + " 332 " + (client) + " " + (channel) + " :" + (topic) + "\r\n"				//332
#define RPL_INVITING(host, client, target, channel) ":" + (host) + " 341 " + (client) + " " + (target) + " :" + (channel) + "\r\n"			//341
#define RPL_NAMREPLY(host, nick, channel, users) ":" + (host) + " 353 " + (nick) + " = " + (channel) + " :" + users +  "\r\n" 				//353
#define RPL_ENDOFNAMES(host, nick, channel) ":" + (host) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n" 				//366
#define RPL_BANLIST(host, client, target, user) (":" + (host) + " 367 " + (client) + " " + (target) \
	+ " " + (user) + " " + (client) + "\r\n")																								//367
#define RPL_ENDOFBANLIST(host, client, target) ":" + (host) + " 368 " + (client) + " " + (target) + " :End of channel ban list" +" \r\n" 	//368

/* Command Success Messages */
#define CMD_JOIN(prefix, channel) ":" + (prefix) + " JOIN :" + (channel) + "\r\n"
#define CMD_NICK(prefix, nick) ":" + (prefix) + " NICK :" + (nick) + "\r\n"
#define CMD_PRIVMSG(prefix, target, message) ":" + (prefix) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
#define CMD_NOTICE(prefix, target, message) ":" + (prefix) + " NOTICE " + (target) + " :" + (message) + "\r\n"
#define CMD_PONG(hostname, token) ":" + (hostname) + " PONG " + (hostname) + " :" + (token) + "\r\n"
#define CMD_PING(hostname, timestamp) ":" + (hostname) + " PING " + (hostname) + " :" + (timestamp) + "\r\n"
#define CMD_PART(prefix, channel, message) ":" + (prefix) + " PART " + (channel) + " :" + (message) + "\r\n"
#define CMD_PART_NO_MSG(prefix, channel) ":" + (prefix) + " PART " + (channel) + "\r\n"
#define CMD_INVITE(prefix, targetnick, channel) ":" + (prefix) + " INVITE " + (targetnick) + " " + (channel) + "\r\n"
#define CMD_MODE(prefix, channel, modes) ":" + (prefix) + " MODE " + (channel) + " :" + (modes) + "\r\n"
#define	CMD_KICK(prefix, channel, target, msg) ":" + (prefix) + " KICK " + (channel) + " " + (target) + " :" + (msg) + "\r\n"
#define CMD_QUIT(nick, user, ip) ":" + (nick) + "!" + (user) + "@" + (ip) + " QUIT :Connection closed" + "\r\n"



/* Error Messages */
#define ERR_UMODEUNKNOWNFLAG(host, client, mode) ":" + (host) + " 501 " + (client) + " " + (mode) + " :is not a recognised user mode" + "\r\n" 					//501
#define ERR_KEYSET(host, client, target) ":" + (host) + " 467 " + (client) + " " + (target) + " :Channel key already set" + "\r\n" 								//467
#define ERR_SHUTDOWN(user, address) "ERROR :Closing link: (" + (user) + "@" + (address) + ") [Server shutting down]\r\n"
#define ERR_NOSUCHNICK(host, client, target) ":" + (host) + " 401 " + (client) + " " + (target) + " :No such nick" + "\r\n" 									//401
#define ERR_NOSUCHCHANNEL(host, client, target) ":" + (host) + " 403 " + (client) + " " + (target) + " :No such channel" + "\r\n" 								//403
#define ERR_UNKNOWNMODE(host, client, mode) ":" + (host) + " 472 " + (client) + " " + (mode) + " :is not a recognised channel mode" + "\r\n" 					//472
#define	ERR_INVITEONLYCHAN(host, client, target) ":" + (host) + " 473 " + (client) + " " + (target) + " :Cannot join channel (invite only)" + "\r\n" 			//473
#define ERR_USERSDONTMATCH(host, nick) ":" + (host) + " 502 " + (nick) + " :Can't view modes for other users" + "\r\n" 											//502
#define ERR_LOSTCONNECT(user, address) "ERROR :Closing link: (" + (user) + "@" + (address) + ") [Connection closed]\r\n"
#define ERR_UNKNOWNCOMMAND(host, client, target) ":" + (host) + " 421 " + (client) + " " + (target) + " :Unknown command" + "\r\n"								//421
#define ERR_NORECIPIENT(host, client, cmd) ":" + (host) + " 411 " + (client) + " :No recipient given (" + (cmd) + ")" + "\r\n"									//411
#define ERR_NOTEXTTOSEND(host, client, target) ":" + (host) + " 412 " + (client) + " " + (target) + " :No text to send" + "\r\n"								//412
#define ERR_NONICKNAMEGIVEN(host) ":" + (host) + " 431 :No nickname given" + "\r\n" 																			//431
#define ERR_ERRONEUSNICKNAME(host, nick) ":" + (host) +  " 432 " + (nick) + " :Erroneous nickname" + "\r\n" 													//432
#define ERR_NICKNAMEINUSE(host, nick) ":" + (host) +  " 433 " + (nick) + " :Nickname is already in use" + "\r\n" 			 									//433
#define ERR_NOTONCHANNEL(host, client, target) ":" + (host) + " 442 " + (target) + " :You're not on that channel" + "\r\n"   									//442
#define ERR_NEEDMOREPARAMS(host, client, target) ":" + (host) + " 461 " + (target) + " :Not enough parameters" + "\r\n" 										//461
#define ERR_ALREADYREGISTRED(host) ":" + (host) + " 462 " + ":Unauthorized command (already registered)" + "\r\n" 												//462
#define	ERR_BANNEDFROMCHAN(host, client, target) ":" + (host) + " 474 " + (client) + " " + (target) + " :Cannot join channel (+b)" + "\r\n"						//474
#define	ERR_BADCHANNELKEY(host, client, target) ":" + (host) + " 475 " + (client) + " " + (target) + " :Cannot join channel (+k)" + "\r\n" 			 			//475
#define ERR_BADCHANMASK(host, client, target) ":" + (host) + " 476 " + (client) + " " + (target) + " :Bad Channel Mask" + "\r\n" 								//476
#define ERR_USERONCHANNEL(host, client, target, channel) ( ":" + (host) + " 443 " + (client) + " " + (target) + " " + (channel) \
	+ " :is already on channel" + "\r\n" )																														//443
#define ERR_CANNOTSENDTOCHAN(host, client, target, msg) ":" + (host) + " 404 " + (client) + " " + (target) + " :You cannot send " + (msg) + "\r\n"				//404
#define ERR_CHANOPRIVSNEEDED(host, client, target, modeMessage) ":" + (host) + " 482 " + (client) + " " + (target) + " " + (modeMessage) + "\r\n"				//482
#define ERR_NOTREGISTERED(host) ":" + (host) + " 451 :You have not registered" + "\r\n" 																		//451
