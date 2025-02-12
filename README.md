# ft_irc
## Description
`ft_irc` is a project focused on creating a custom [IRC (Internet Relay Chat)](https://en.wikipedia.org/wiki/IRC) server in C++. The project aims to develop a functional IRC server capable of handling multiple client connections using TCP/IP while following established IRC standards. The server must manage user authentication, message forwarding, and channel operations. This server has been developed and tested with [irssi](https://irssi.org/).

## Features
- Handles multiple client connections simultaneously.
- Implements authentication with a password.
- Allows users to set nicknames, usernames, and join channels.
- Supports private messaging and message forwarding.
- Includes channel operator commands:
  - `KICK` - Remove a client from a channel.
  - `INVITE` - Invite a user to a channel.
  - `TOPIC` - Set or view the channel topic.
  - `MODE` - Manage channel modes (invite-only, topic restrictions, password, operator privileges, user limits).
- Uses non-blocking I/O and `poll()` (or equivalent) for efficient event handling.

## Installation
1. Clone the repository:
```
git clone https://github.com/ito-miyuki/ft_irc.git
```
2. Navigate to the project directory:
```
cd ft_irc
```
3. Compile the project:
```
make
```
## Usage
Run the server with:
```
./ircserv <port> <password>
```
Example:
```
./ircserv 6667 mypassword
```
Connect to the server using an IRC client:
```
/connect 127.0.0.1 6667 mypassword
```
## Testing
Use `nc` to manually send commands:
```
nc 127.0.0.1 6667
```
Then type commands like:
```
PASS mypassword
NICK user1
USER user1 0 * :Real Name
JOIN #channel
```
