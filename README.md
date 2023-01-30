# ft_irc

ft_irc is a basic IRC server written in C++98, developed as a team project for the school 42 Quebec.
It was developed in collaboration with [jgoad](https://github.com/jongoad) and [spawnzer](https://github.com/spawnzer)

## Usage

- ft_irc currently only works on MacOS.
- To build the executable, simply run
    
    ```bash
    $> make && ./ircserv <port> <password>
    ```
    

## Troubleshooting

If you encounter any issues while using ft_irc, please contact us via the [Issues](https://github.com/oddtiming/ft_irc/issues) page.

## Supported Commands
- `AWAY [message]` 
  - Set the user's away status with an optional message, indicating that the user is unavailable.
  - If no message is provided, away status is removed.
- `INVITE <nick> <channel>`
  - Invite target <nick> to join <channel>.
- `JOIN <channel>( "," <channel> )  [key] ( "," [key] )`
  - Join the target <channel>, using an optional key for password-protected channels.
- `KICK <channel> <user> [reason]`
  - Remove target <user> from <channel> with an optional reason for the kick.
- `LIST [channel]`
  - List all channels on the server, or display information about the specified target <channel>.
- `MODE <target1> [modes] [target2]`
  - Set modes for the target1, which can be either a user or a channel.
  - If target1 is a channel, target2 must be a user. Only applies to mode +b (ban).
  - See [Supported user modes](#supported-user-modes) and [Supported channel modes](#supported-channel-modes) for full list of supported modes.
- `NAMES [channel]`
  - List the users in the target <channel>, or in all channels if no target is specified.
- `NICK <nick>`
  - Change the user's current nickname to <nick>.
- `NOTICE <target> <message>`
  - Send a <message> to the target, which can be either a user or a channel.
- `PART <channel> [message]`
  - Leave target <channel> with an optional <message> to be displayed to everyone on the channel.
- `PASS <password>`
  - Provide a password for authentication purposes.
- `PING <server>`
  - Request a response from the target <server> to check if it is still connected.
- `PONG <server>`
  - Reply to a PING request, confirming that the server is still connected.
- `PRIVMSG <target> <message>`
  - Send a <message> to the target, which can be either a user or a channel.
- `QUIT [message]`
  - Disconnect from the server with an optional <message> to be displayed to all channels the user is on.
- `SHUTDOWN [message]`
  - Shut down the server with an optional <message>.
- `TOPIC <channel> [topic]`
  - Set or display the topic for the target <channel>.
- `USER <user> <mode> <unused> <real name>`
  - Provide information about the user, including username, mode, and real name.
- `WHO <target>`
  - Display information about the specified <target>.
  - If target is a channel, all visible members will be listed.
  - If target is a user, all visible channels it is on will be listed.
- `WHOIS <nick>`
  display information about the target <nick>, including channels the user is in, away status, and real name.


## Supported user modes

- `INVIS` (+i)
  - Invisible: user cannot be seen unless they are known)
- `BAN` (+b)
  - Ban: prevent user from joining channel
- `C_OP` (+o)
	- Operator: user can set channel modes as well as user modes for other users
- `OWNER` (+q)
  - Owner: can only be attributed to the first connected user. 
    - If that user leaves, and the channel is not empty, status is given to the highest ranked user.
      - If multiple users have the same rank, owner status is given to oldest user.

## Supported channel modes
- `SECRET` (+s)
	- Secret channel: extends private channel functionality, cannot be seen by queries for TOPIC, LIST, NAMES. 
- `TOPIC_SET_OP` (+t)
	- Topic limit (only +qo can set topic)
	- **SET BY DEFAULT**
- `INV_ONLY` (+i)
	- Invite only: users may only join channel if they have been invited by +qo. 
- `NO_MSG_IN` (+n)
  - No external messages: external messages cannot be seen in channel.
- `PASS_REQ` (+k)	
  - Password protected: channel requires a password to join.

  
  
## Bots

- Three bots have been coded as part of the project: `DictBot`, `JokeBot`, and `TimeBot`
- If you want to launch the bots, you can run the following command once the server is running:

```bash
  $> make bots
```
- The configuration options for the bots can be found in their [main file](https://github.com/oddtiming/ft_irc/blob/main/bots/srcs/main.cpp).
- Bots can also be launched independently by running the following commands in the terminal:

```bash
  $> cd bots && make
  $> ./ircBot ["dict"/"joke"/"time"]
```
- Once in the same channel as the bots, users can activate them by sending a `!dict <word>`, `!joke`, or `!time` message to the channel.

### DictBot
  - Activated with `!dict <word>` command.
  - Provides the first 5 Oxford Dictionary definitions for the inputted word.
  - **Requires to [register with the Oxford Dictionaries API](https://developer.oxforddictionaries.com/#plans).**
    - Once this is done, set `OXFORD_APP_KEY` and `OXFORD_APP_ID` environment variables, and launch `DictBot` again.

  ```bash
    $> export OXFORD_APP_ID="your_app_id"
    $> export OXFORD_APP_KEY="your_app_key"
    $> ./ircBot "dict"
  ```

### JokeBot
  - Activated with `!joke` command.
  - Provides a random joke from the [free joke API](https://jokeapi.dev/joke/Any).
  - Does not require any API authentication.
  - **Warning**: Some of these are pretty NSFW, but the request does warn you. Feel free to update the code to ignore those.

### TimeBot

  - Activated with `!time` command.
  - Provides the requesting user with the current time.
