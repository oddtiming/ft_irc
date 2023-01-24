#include "Command.hpp"

/* Reply Messages */
#define RPL_WELCOME(nick, prefix) "001 " + (nick) + " Welcome to Internet Relay Network " + (prefix) + "\r\n" //001
#define RPL_UMODEIS(modes) "221 * " + (modes) + "\r\n" //221
#define RPL_AWAY(hostname, client, target, message) ":" + (hostname) + " 301 " +  (client) + " " + (target) + " :" + (message) + "\r\n"
#define RPL_UNAWAY()  "305 * :You are no longer marked as being away \r\n" //305
#define RPL_NOWAWAY() "306 * :You have been marked as being away \r\n" //306
#define RPL_WHOISUSER(nick, user, host, realName) "311 * " + (nick) + " " + (user) + " " + (host) + " * :"+ (realName) + "\r\n" //311
#define RPL_WHOISSERVER(nick, server, serverName) "312 * " + (nick) + " " + (server) + " :" + (serverName) + "\r\n" //312
#define RPL_WHOISIDLE(nick, idle) "317 * " + (nick) + " " + (idle) + " :seconds idle\r\n" //317
#define RPL_ENDOFWHOIS(nick) "318 * " + (nick) + " :End of WHOIS list\r\n" //318
#define RPL_WHOISCHANNELS(msg) "319 * " + (msg) + "\r\n" //319
#define RPL_LIST(channel,nbUsers,topic) "322 * " + (channel) + " " + (nbUsers) + " : " + (topic) +  "\r\n" //322
#define RPL_ENDOFNAMES(host, nick, channel) ":" + (host) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n" //366
#define RPL_LISTEND(host, nick) ":" + (host) + " 323 " + (nick) + " :End of channel list.\r\n" //323
#define RPL_CHANNELMODEIS(target, modes, param) "324 * " + (target) + " " + (modes) + param + "\r\n" //324
#define	RPL_NOTOPIC(channel) "331 * " + (channel) + " :No topic is set \r\n" //331
#define	RPL_TOPIC(channel, topic) "332 * " + (channel) + " :" + (topic) + "\r\n"//332
//#define RPL_INVITING(channel,nick) "341 * " + (channel) + " " + (nick) + "\r\n" //341
#define RPL_NAMREPLY(hostname, nick, channel, users) ":" + (hostname) + " 353 " + (nick) + " = " + (channel) + " :" + users +  "\r\n" //353
#define RPL_INVITING(prefix, targetnick, channel) ":" + (prefix) + " INVITE " + (targetnick) + " " + (channel) + "\r\n"

/* Command Success Messages */
#define CMD_JOIN(prefix, channel) ":" + (prefix) + " JOIN :" + (channel) + "\r\n"
#define CMD_NICK(prefix, nick) ":" + (prefix) + " NICK :" + (nick) + "\r\n"
#define CMD_PRIVMSG(prefix, target, message) ":" + (prefix) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
#define CMD_NOTICE(prefix, target, message) ":" + (prefix) + " NOTICE " + (target) + " :" + (message) + "\r\n"
#define CMD_PONG(hostname, token) ":" + (hostname) + " PONG " + (hostname) + " :" + (token) + "\r\n"
#define CMD_PING(hostname, timestamp) ":" + (hostname) + " PING " + (hostname) + " :" + (timestamp) + "\r\n"
#define CMD_PART(prefix, channel, message) ":" + (prefix) + " PART " + (channel) + " :" + (message) + "\r\n"
#define CMD_PART_NO_MSG(prefix, channel) ":" + (prefix) + " PART " + (channel) + "\r\n"
#define CMD_INVITE(hostname, client, target, channel) ":" + (hostname) + " 341 " + (client) + " " + (target) + " :" + (channel) + "\r\n"
// FIXME: the RPL_INVITING and CMD_INVITE are curently inverted; 
//  the 341 reply code is meant to be sent to the inviting client, not the invited client

/* Error Messages */
#define ERR_NOSUCHNICK(nickname) "401 * " + (nickname) + " :No such nickname" + "\r\n" //401
#define ERR_CANNOTSENDTOCHAN(channel) "404 * " + (channel) + " :Cannot send to channel" + "\r\n" //404
#define ERR_NOSUCHCHANNEL(channel) "403 * " + (channel) + " :No such channel" + "\r\n" //403
#define	ERR_TOOMANYCHANNELS(channel) (channel) + " :You have joined too many channels" //405
#define ERR_NORECIPIENT(cmd) ":No recipient given (" + (cmd) + ")" //411
#define ERR_NOTEXTTOSEND() "412 * :No text to send\r\n" //412
#define ERR_UNKNOWNCOMMAND(cmd) "421 * " + cmd + " :Unknown command\r\n" //421
#define ERR_NONICKNAMEGIVEN() "431 * :No nickname given \r\n" //431
#define ERR_ERRONEUSNICKNAME(nick) "432 * " + (nick) + " :Erroneous nickname" + "\r\n" //432
#define ERR_NICKNAMEINUSE(nick) ":433 * " + (nick) + " :Nickname is already in use" + "\r\n" //433
#define ERR_NOTONCHANNEL(channel) (channel) + " :You're not on that channel" + "\r\n"
#define ERR_NEEDMOREPARAMS(cmd) "461 * " + (cmd) + " :Not enough parameters" + "\r\n" //461
#define ERR_ALREADYREGISTRED() ":Unauthorized command (already registered) \r\n" //462
#define	ERR_CHANNELISFULL(channel) (channel) + " :Cannot join channel (+l)" //471
#define ERR_UNKNOWNMODE(mode, channel) "472 * " + (mode) + " :is unknown mode char to me for " + (channel) + "\r\n" //472
#define	ERR_INVITEONLYCHAN(channel) (channel) + " :Cannot join channel (+i)" //473
#define	ERR_BANNEDFROMCHAN(channel) "474 * " + (channel) + " :Cannot join channel (+b)" + "\r\n"//474
#define	ERR_BADCHANNELKEY(channel) (channel) + " :Cannot join channel (+k)" //475
#define ERR_BADCHANMASK(channel) (channel) + " :Bad Channel Mask" //476
#define ERR_CHANOPRIVSNEEDED(channel) "482 * " + (channel) + " :You're not channel operator" + "\r\n"//482
#define ERR_USERONCHANNEL(nickname, channel) ":hostname 443 " + (nickname) + " " + (channel) + " :is already on channel\r\n"  

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