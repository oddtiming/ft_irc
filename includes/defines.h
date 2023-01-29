#ifndef DEFINES_H
#define DEFINES_H

#pragma once

/* General server settings */
#define MAX_BUFFER_SIZE 512		/* Maximum read size from recv() in bytes */
#define MAX_CHANNELS    100		/* Maximum number of channels that can exist on server */
#define PING_INTERVAL   60		/* Interval after which to send a ping to client since their last activity */
#define MAX_CONNECTIONS 10

/* Global Modes */
typedef enum e_globalModes {
	AWAY  = 0x1, 				/* a: away (user mode is set as away) */
	INVIS = 0x2 				/* i: invisible (user cannot be seen unless they are known) */
} t_globalModes;

/* Channel Modes */
typedef enum e_channelModes {
	SECRET       = 0x1,  		/* s: secret channel (extends private channel functionality, cannot be seen by queries for TOPIC, LIST, NAMES) */
	TOPIC_SET_OP = 0x2,  		/* t: topic limit (only +qao can set topic) SET DEFAULT */
	INV_ONLY     = 0x4,  		/* i: invite only (users may only join channel if they have been invited by +qao) */
	NO_MSG_IN    = 0x8,  		/* n: no external messages (external messages cannot be seen in channel) */
	PASS_REQ     = 0x10, 		/* k: password protected (channel requires a password to join) */
} t_channelModes;

/* Channel Member Modes */
typedef enum e_memberModes {
	BAN   = 0x1, 				/* b: ban (prevent user from joining channel) */
	INVIT = 0x2, 				/* invited (user has been invite to join this channel) */
	C_OP  = 0x4, 				/* o: operator */
	OWNER = 0x8  				/* q: owner */
} t_memberModes;

/* Other enums */
typedef enum e_targetType { CHANNEL = 0x1, USER = 0x2 } t_targetType;
typedef enum s_serverStatus { OFFLINE = 0, ONLINE = 1, CLOSED = 2 } t_serverStatus;


/* Colour defines for terminal output */
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define CLEAR   "\033[0m"

#endif