#include "Command.hpp"

/* Reply Messages */

/* TESTED */
#define RPL_NOWAWAY(host, client) ":" + (host) + " 306 " + (client) + " :You have been marked as being away" + "\r\n" 						//306
#define RPL_WELCOME(host, client, prefix) ":" + (host) +  " 001 " + (client) + " :Welcome to the IRC server " + (prefix) + "\r\n" 			//001
#define RPL_UMODEIS(host, client, modes) ":" + (host) + " 221 " + (client) + " :" + (modes) + "\r\n" 										//221
#define RPL_AWAY(hostname, client, target, message) ":" + (hostname) + " 301 " +  (client) + " " + (target) + " :" + (message) + "\r\n" 	//301
#define RPL_UNAWAY(host, client) ":" + (host) + " 305 " + (client) + " :You are no longer marked as being away" + "\r\n" 					//305
#define RPL_WHOISUSER(host, client, nick, user, address, real) ":" + (host) + " 311 " + (client) + " " + (nick) + " " + (user) + " " + (address) + " * :"+ (real) + "\r\n" 			//311

/* NOT TESTED */

#define RPL_WHOISSERVER(nick, server, serverName) "312 * " + (nick) + " " + (server) + " :" + (serverName) + "\r\n" 						//312
#define RPL_WHOISIDLE(nick, idle) "317 * " + (nick) + " " + (idle) + " :seconds idle\r\n" 													//317
#define RPL_ENDOFWHOIS(nick) "318 * " + (nick) + " :End of WHOIS list\r\n" 																	//318
#define RPL_WHOISCHANNELS(msg) "319 * " + (msg) + "\r\n" 																					//319
#define RPL_LIST(channel,nbUsers,topic) "322 * " + (channel) + " " + (nbUsers) + " : " + (topic) +  "\r\n" 									//322
#define RPL_LISTEND(host, nick) ":" + (host) + " 323 " + (nick) + " :End of channel list.\r\n" 												//323
#define RPL_CHANNELMODEIS(host, nick, channel, modes) ":" + (host) + " 324 " + (nick) + " " + (channel) + " :" + (modes) + "\r\n" 			//324
#define	RPL_NOTOPIC(channel) "331 * " + (channel) + " :No topic is set \r\n" 																//331
#define	RPL_TOPIC(channel, topic) "332 * " + (channel) + " :" + (topic) + "\r\n"															//332
#define RPL_INVITING(hostname, client, target, channel) ":" + (hostname) + " 341 " + (client) + " " + (target) + " :" + (channel) + "\r\n"	//341
#define RPL_NAMREPLY(hostname, nick, channel, users) ":" + (hostname) + " 353 " + (nick) + " = " + (channel) + " :" + users +  "\r\n" 		//353
#define RPL_ENDOFNAMES(host, nick, channel) ":" + (host) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n" 				//366
#define RPL_BANLIST(nick, channel, target) "367 " + (nick) + " " + (channel) + " " + (target) + "\r\n" 										//367
#define RPL_ENDOFBANLIST(nick, channel) "368 " + (nick) + " " + (channel) + " :End of channel ban list.\r\n" 								//368


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






/* Error Messages */

/* TESTED */
#define ERR_UMODEUNKNOWNFLAG(host, client, mode) ":" + (host) + " 501 " + (client) + " " + (mode) + " :is not a recognised user mode" + "\r\n" 					//501
#define ERR_KEYSET(host, client, target) ":" + (host) + " 467 " + (client) + " " + (target) + " :Channel key already set" + "\r\n" 								//467
#define ERR_SHUTDOWN(user, address) "ERROR :Closing link: (" + (user) + "@" + (address) + ") [Server shutting down]\r\n"
#define ERR_NOSUCHNICK(host, client, target) ":" + (host) + " 401 " + (client) + " " + (target) + " :No such nick" + "\r\n" 									//401
#define ERR_NOSUCHCHANNEL(host, client, target) ":" + (host) + " 403 " + (client) + " " + (target) + " :No such channel" + "\r\n" 								//403
#define ERR_UNKNOWNMODE(host, client, mode) ":" + (host) + " 472 " (client) + " " + (mode) + " :is not a recognised channel mode" + "\r\n" 						//472
#define	ERR_INVITEONLYCHAN(host, client, target) ":" + (host) + " 473 " + (client) + " " (target) + " :Cannot join channel (invite only)" + "\r\n" 				//473
#define ERR_USERSDONTMATCH(host, nick) ":" + (host) + " 502 " + (nick) + " :Can't view modes for other users" + "\r\n" 											//502



/* TO BE TESTED */
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
#define ERR_BADCHANMASK(host, client, target) ":" + (host) + " 476 " + (client) + " " + (channel) + " :Bad Channel Mask" + "\r\n" 								//476
#define CHANOPMSG " :You must be a channel op or higher to set channel mode "
#define ERR_USERONCHANNEL(host, client, target, channel) ":" + (host) + " 443 " + (client) + " " (nick) + " " + (channel) + " :is already on channel" + "\r\n"	//443


/* TO BE FIXED */

//FIXME: 404 reply can support multiple types of replies
// :penguin.omega.example.org 404 jon #new2 :You cannot send messages to this channel whilst the +m (moderated) mode is set.
// :penguin.omega.example.org 404 todd #new2 :You cannot send messages to this channel whilst banned.
#define CANNOTSENDMSG " :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set"												//FIXME: Make sure to test this when mode +n is fixed
#define ERR_CANNOTSENDTOCHAN(host, client, target) ":" + (host) + " 404 " + (client) + " " + (target) + CANNOTSENDMSG + "\r\n"									//404


//FIXME: 482 reply can support multiple types of replies
// :penguin.omega.example.org 482 Bill #new :You must be a channel op or higher to send an invite.
// :penguin.omega.example.org 482 jon #new2 :You must be a channel op or higher to set channel mode m (moderated).
// :penguin.omega.example.org 482 jon #new2 :You must be a channel op or higher to set channel mode m (moderated).
// :penguin.omega.example.org 482 jon #new2 :You must be a channel op or higher to set channel mode i (inviteonly).
// :penguin.omega.example.org 482 jon #new2 :You must be a channel op or higher to set channel mode s (secret).
// :penguin.omega.example.org 482 jon #new2 :You must be a channel op or higher to set channel mode m (moderated).
#define ERR_CHANOPRIVSNEEDED(host, client, target, mode) ":" + (host) + " 482 " + (client) + " " + (target) + CHANOPMSG + (mode) + "\r\n"						//482




/* NOT IMPLEMENTED */

// :penguin.omega.example.org 696 jon #new k * :You must specify a parameter for the key mode. Syntax: <key>.
// :penguin.omega.example.org 698 jon #new bill b :Channel ban list does not contain bill







/*

#define RPL_WELCOME 1
#define RPL_YOURHOST 2
#define RPL_CREATED 3
#define RPL_MYINFO 4
#define RPL_BOUNCE 5
#define RPL_TRACELINK 200
#define RPL_TRACECONNECTING 201
#define RPL_TRACEHANDSHAKE 202
#define RPL_TRACEUNKNOWN 203
#define RPL_TRACEOPERATOR 204
#define RPL_TRACEUSER 205
#define RPL_TRACESERVER 206
#define RPL_TRACENEWTYPE 208
#define RPL_STATSLINKINFO 211
#define RPL_UMODEIS 221
#define RPL_STATSLLINE 241
#define RPL_STATSUPTIME 242
#define RPL_STATSOLINE 243
#define RPL_STATSHLINE 244
#define RPL_LUSERCLIENT 251
#define RPL_LUSEROP 252
#define RPL_LUSERUNKNOWN 253
#define RPL_LUSERCHANNELS 254
#define RPL_LUSERME 255
#define RPL_ADMINME 256
#define RPL_ADMINLOC1 257
#define RPL_ADMINLOC2 258
#define RPL_ADMINEMAIL 259
#define RPL_TRACELOG 261
#define RPL_TRACEEND 262
#define RPL_TRYAGAIN 263
#define RPL_NONE 300
#define RPL_AWAY 301
#define RPL_USERHOST 302
#define RPL_ISON 303
#define RPL_UNAWAY 305
#define RPL_NOWAWAY 306
#define RPL_WHOISUSER 311
#define RPL_WHOISSERVER 312
#define RPL_WHOISOPERATOR 313
#define RPL_WHOWASUSER 314
#define RPL_ENDOFWHO 315
#define RPL_WHOISIDLE 317
#define RPL_ENDOFWHOIS 318
#define RPL_WHOISCHANNELS 319
#define RPL_LISTSTART 321
#define RPL_LIST 322
#define RPL_LISTEND 323
#define RPL_CHANNELMODEIS 324
#define RPL_UNIQOPIS 325
#define RPL_NOTOPIC 331
#define RPL_TOPIC 332
#define RPL_INVITING 341
#define RPL_SUMMONING 342
#define RPL_INVITELIST 346
#define RPL_ENDOFINVITELIST 347
#define RPL_EXCEPTLIST 348
#define RPL_ENDOFEXCEPTLIST 351
#define RPL_VERSION 352
#define RPL_WHOREPLY 353
#define RPL_NAMREPLY 364
#define RPL_LINKS 365
#define RPL_ENDOFLINKS 366
#define RPL_ENDOFNAMES 367
#define RPL_BANLIST 368
#define RPL_ENDOFBANLIST 369
#define RPL_ENDOFWHOWAS 371
#define RPL_INFO 372
#define RPL_MOTD 374
#define RPL_ENDOFINFO 375
#define RPL_MOTDSTART 376
#define RPL_ENDOFMOTD 381
#define RPL_YOUREOPER 382
#define RPL_REHASHING 383
#define RPL_YOURESERVICE 391
#define RPL_TIME 392
#define RPL_USERSSTART 393
#define RPL_USERS 394
#define RPL_ENDOFUSERS 395
#define RPL_NOUSERS 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_WASNOSUCHNICK 406
#define ERR_TOOMANYTARGETS 407
#define ERR_NOSUCHSERVICE 408
#define ERR_NOORIGIN 409
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413
#define ERR_WILDTOPLEVEL 414
#define ERR_BADMASK 415
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NOMOTD 422
#define ERR_NOADMININFO 423
#define ERR_FILEERROR 424
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOLOGIN 444
#define ERR_SUMMONDISABLED 445
#define ERR_USERSDISABLED 446
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_NOPERMFORHOST 463
#define ERR_PASSWDMISMATCH 464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_KEYSET 467
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_NOCHANMODES 477
#define ERR_BANLISTFULL 478
#define ERR_NOPRIVILEGES 481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_CANTKILLSERVER 483
#define ERR_RESTRICTED 484
#define ERR_UNIQOPPRIVSNEEDED 485
#define ERR_NOOPERHOST 491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502
*/