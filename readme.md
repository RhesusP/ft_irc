<div align="center">

# ft_irc

#### My own Internet Relay Chat server

<br />
<br />
<img src="https://42f2671d685f51e10fc6-b9fcecea3e50b3b59bdc28dead054ebc.ssl.cf5.rackcdn.com/illustrations/Chat_re_re1u.svg" height=300>
<br />
<br />
</div>
<br />

## About

This project was carried out as part of the [School 42](https://42.fr/en/homepage/) Common Core. The aim of this project
is to discover the [IRC protocol](https://fr.wikipedia.org/wiki/Internet_Relay_Chat) and to create our own IRC server
using c++98.

## Authors

| Author                                | GitHub profile                         |
|---------------------------------------|----------------------------------------|
| Samuel Vanmeenen Bachelard (svanmeen) | [@Lyptis](https://github.com/Lyptis)   |
| Camille Bernot (cbernot)              | [@RhesusP](https://github.com/RhesusP) |

## Grade : 125/100 ✅✨

## Subject requirements

- [x] Take a port and a password as parameters.
- [x] The server must handle several clients at the same time without being blocked.
- [x] Multi-threading is not allowed.
- [x] Server must use only one `poll`
- [x] Communication between the server and clients must be done using TCP/IP sockets.
- [x] User must be able to set a nickname and a username, join a channel, send and receive messages (public or private).
- [x] Server must be able to handle several channels.
- [x] Regular users and channel operators must be implemented.
- [x] The following commands must be implemented : `KICK`, `INVITE`, `TOPIC` and `MODE` (with modes `i`, `t`, `k`, `o`
  and `l`).
- [x] Server must be able to receive partial messages.

## Commands implemented

| Command   | Description                                                                                                                                                                                                                                                                                                                                                                                                                                       |
|-----------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `PASS`    | Set a connection password to begin the registration process                                                                                                                                                                                                                                                                                                                                                                                       |
| `USER`    | Specify a username and realname of a new user during registration process                                                                                                                                                                                                                                                                                                                                                                         |
| `NICK`    | Set the user a nickname (or change the previous one)                                                                                                                                                                                                                                                                                                                                                                                              |
| `MOTD`    | Display the Message Of The Day of the server                                                                                                                                                                                                                                                                                                                                                                                                      |
| `JOIN`    | Join one or several channels                                                                                                                                                                                                                                                                                                                                                                                                                      |
| `KICK`    | Force remove a user from a channel                                                                                                                                                                                                                                                                                                                                                                                                                |
| `MODE`    | Display or set channel mode : <br/>- `+k` set a password to the channel<br/>- `-k` remove the password<br/>- `+i` set the channel as invite-only<br/>- `-i` remove the invite-only restriction<br/>- `+t` protect the channel topic<br/>- `-t` remove topic protection<br/>- `+o` give channel operator privileges to a user<br/>- `-o` remove channel operator privileges to a user<br/>- `+l` set a user limit<br/>- `-l` remove the user limit |
| `TOPIC`   | Display of set the channel topic                                                                                                                                                                                                                                                                                                                                                                                                                  |
| `INVITE`  | Invite a user to a channel                                                                                                                                                                                                                                                                                                                                                                                                                        |
| `PART`    | Remove the user from the given channel(s)                                                                                                                                                                                                                                                                                                                                                                                                         |
| `QUIT`    | Terminate client's connection to the server                                                                                                                                                                                                                                                                                                                                                                                                       |
| `PRIVMSG` | Send a message to a user or a channel                                                                                                                                                                                                                                                                                                                                                                                                             |
| `WHO`     | Display infos about users                                                                                                                                                                                                                                                                                                                                                                                                                         |
| `PING`    | Check that the server is still connected to the client                                                                                                                                                                                                                                                                                                                                                                                            |
| `BOT`     | Activate or deactivate a bot that greets users in a channel                                                                                                                                                                                                                                                                                                                                                                                       |

## Usage

```shell
make && ./ircserv <port> <password>
```

## Useful Links

- [Internet Relay Chat Protocol (RFC1459)](https://datatracker.ietf.org/doc/html/rfc1459)
- [Internet Relay Chat: Architecture (RFC2810)](https://datatracker.ietf.org/doc/html/rfc2810)
- [Internet Relay Chat: Channel Management (RFC2811)](https://datatracker.ietf.org/doc/html/rfc2811)
- [Internet Relay Chat: Client Protocol (RFC2812)](https://datatracker.ietf.org/doc/html/rfc2812)
- [RFC1459 (French version)](http://abcdrfc.free.fr/rfc-vf/rfc1459.html)
- [Internet Relay Chat: Server Protocol (RFC2813)](https://datatracker.ietf.org/doc/html/rfc2813)
- [Default Port for Internet Relay Chat (IRC) via TLS/SSL (RFC7194)](https://datatracker.ietf.org/doc/html/rfc7194)
- [The UChicago X-Projects](http://chi.cs.uchicago.edu/chirc/index.html)
- [ircdocs.horse](https://modern.ircdocs.horse/#connection-setup)
- [Guide to Network Programming](https://beej.us/guide/bgnet/html/)
