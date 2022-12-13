#ifndef DEFINES_H
# define DEFINES_H

#pragma once



/* Global Modes */
typedef enum e_globalModes {
	OP = 0x1,
	AWAY = 0x2
};


/* Channel Modes */
typedef enum e_channelModes {
	PRIVATE = 			0x1,		/* Channel is private */
	SECRET = 			0x2,		/* Channe is secred (operator only) */
	MODERATED =			0x4,		/* Channel is moderated */
	INV_ONLY =			0x8,		/* Channel is invite only */
	TOPIC_SET_OP =		0x10,		/* Only operators can set topic */
	NO_MSG_IN = 		0x20		/* Prevent incoming messages to channel */
};

/* Channel Member Modes */
typedef enum e_memberModes {
	OP = 				0x1,		/* User is operator on channel */
	BAN = 				0x2			/* User is banned from channel */
	//Give owner status to user

};

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