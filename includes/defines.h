#ifndef DEFINES_H
# define DEFINES_H

#pragma once


// FIXME: set back to 0 before corrections, makes use of the recvmsg() function
#define DEBUG 1

#define MAX_BUFFER_SIZE 512			/* Maximum read size from recv() in bytes */


/* Global Modes */
typedef enum e_globalModes {
	OP = 				0x1,
	AWAY = 				0x2,
	INVIS = 			0x4
}	t_globalModes;


/* Channel Modes */
typedef enum e_channelModes {
	PRIVATE = 			0x1,		/* -p: private channel (channel cannot be seen unless member, or server op - channel omitted from WHOIS) */
	SECRET = 			0x2,		/* -s: secret channel (extends private channel functionality, cannot be seen by queries for TOPIC, LIST, NAMES) */
	MODERATED =			0x4,		/* -m or -M: moderated channel (only users with +qaov can speak) */
	INV_ONLY =			0x8,		/* -i: invite only (users may only join channel if they have been invited by +qao) */
	TOPIC_SET_OP =		0x10,		/* -t: topic limit (only +qao can set topic) SET DEFAULT */
	NO_MSG_IN = 		0x20,		/* -n: no external messages (external messages cannot be seen in channel) SET BY DEFAULT */
}	t_channelModes;


/* Channel Member Modes */
typedef enum e_memberModes {
	BAN = 				0x1,		/* b: ban (prevent user from joining channel, kick them if they are already in channel) */
	VOICE =				0x2,		/* v: voice (allows user to speak in +m and +M channels) */
	C_OP =				0x4,		/* o: operator (3rd highest perm level) */
	OWNER =				0x8			/* q: owner (highest perm level, only one can exist) */
}	t_memberModes;

//NOTE: All +qao members automatically inherit (VOICE | BAN_EXEMPT | INV_EXEMPT)
//They cannot be placed on ban list, never need an invite, and can always speak



/* Colour defines for terminal */

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define CLEAR "\033[0m"

#endif





/* Modes List

/USER

#define REGISTERED		0b00000001
#define INVISIBLE		0b00000010
#define RECEIVENOTICE	0b00000100
#define RECEIVEWALLOPS	0b00001000
#define IRCOPERATOR		0b00010000
#define AWAY			0b00100000
#define PINGING			0b01000000
#define BREAKCONNECTION	0b10000000


1 2 4 8 10 20 40 80 100 200 400 
____________________________________________________
Global:
A = Server/Net Admin only channel (settable by Admins)

____________________________________________________
Channel Member Modes:
[h] requires at least halfop, [o] requires at least chanop, [q] requires owner

a = Gives Channel Admin to the user
q = Gives Owner status to the user
b = Bans the nick!ident@host from the channel.
       Extended bantypes are also available. (for more info see extended bans tutorial) [h]
e = Overrides a ban for matching users [h]
i = A user must be invited to join the channel [h]
I = Overrides +i for matching users [h]
h = Gives HalfOp status to the user (Limited op access)

Maybe:
k = Users must specify to join (for more info see Using Channel Keys) [h]
v = Gives Voice to the user (May talk if chan is +m)
______________________________________________________
Channel Modes:
l = Channel may hold at most of users [o]

Maybe:
t = Only +hoaq may change the topic [h]
T = No NOTICEs allowed in the channel [o]
u = Auditorium mode (/names and /who #channel only show channel ops) [q]
U = Block messages containing mIRC formatting codes [o]
S = Strips mIRC color codes [o]
r = The channel is registered (settable by services only)
R = Only registered (+r) users may join the channel [o]
p = Private channel (for more info see Using Channel Modes to Hide Your Channel) [o]
O = IRC Operator only channel (settable by IRCops)
L = Channel link (If +l is full, the next user will auto-join ) [q]
m = Moderated channel (only +vhoaq users may speak) [h]
M = Must be using a registered nick (+r), or have voice access to talk [o]
n = Users outside the channel can not send PRIVMSGs to the channel [h]
K = /KNOCK is not allowed [o]
N = No Nickname changes are permitted in the channel [o]
o = Gives Operator status to the user
Q = No kicks allowed [o]
s = Secret channel (for more info see Using Channel Modes to Hide Your Channel) [o]
V = /INVITE is not allowed [o]




*/