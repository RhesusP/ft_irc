/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:05:15 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 13:23:36 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define RPL_WELCOME(nick, hostname, client_identity)    "001 " + nick + " :Welcome to the " + hostname + " Network, " + client_identity + "\r\n"

#define RPL_YOURHOST(nick, hostname)                    "002 " + nick + " :Your host is " + hostname + ", running version 1\r\n"

#define RPL_CREATED(nick, datetime)                        "003 " + nick + " :This server was created " + datetime + "\r\n"

#define RPL_ISUPPORT(nick, support)                        "005 " + nick + " " + support + "\r\n"

#define RPL_LUSERCLIENT(nick, user_nb)                    "251 " + nick + " :There are " + user_nb + " users on 1 server\r\n"

#define RPL_LUSERME(nick, cli_nb)                        "255 " + nick + " :I have " + cli_nb + " clients and 1 server\r\n"

#define RPL_MOTDSTART(nick)                                "375 " + nick + " :irc_serv Message of the Day - \r\n"

#define RPL_MOTD(nick, line)                            "372 " + nick + " :" + line + "\r\n"

#define RPL_ENDOFMOTD(nick)                                "376 " + nick + " :End of Message of the Day\r\n"

#define RPL_TOPIC(nick, channel, topic)                    "332 " + nick + " " + channel + " :" + topic + "\r\n"

#define RPL_NOTOPIC(nick, channel)                        "331 " + nick + " " + channel + " :No topic is set\r\n"

#define RPL_TOPICWHOTIME(nick, channel, setat)        "333 " + nick + " " + channel + " " + nick + " " + setat + "\r\n"

#define RPL_NAMREPLY(nick, channel, name)                "353 " + nick + " = " + channel + " :" + name + "\r\n"

#define RPL_ENDOFNAMES(nick, channel)                    "366 " + nick + " " + channel + " :End of /NAMES list\r\n"

#define RPL_UMODEIS(nick, mode)                            "221 " + nick + " " + mode + "\r\n"

#define RPL_CHANNELMODEIS(nick, channel, mode)            "324 " + nick + " " + channel + " " + mode + "\r\n"

#define RPL_CREATIONTIME(nick, channel, timestamp)            "329 " + nick + " " + channel + " " + timestamp + "\r\n"

#define RPL_INVITING(nick, target, channel)                "341 " + nick + " " + target + " " + channel + "\r\n"

#define RPL_INVITELIST(nick, channel)                    "346 " + nick + " " + channel + "\r\n"

#define RPL_ENDOFINVITELIST(nick)                        "347 " + nick + " :End of /INVITE list\r\n"

#define RPL_PRIVMSG(target, text)                        "PRIVMSG " + target + " :" + text + "\r\n"

#define RPL_WHOREPLY(nick, channel, username, host, nickname, flags, realname)    "352 " + nick + " " + channel + " " + username + " " + host + " " + SERVER_NAME + " " + nickname + " " + flags + " :0 " + realname + "\r\n"

#define RPL_ENDOFWHO(nick, mask)                        "315 " + nick + " " + mask + " :End of /WHO list\r\n"

#define RPL_QUIT(reason)                                (std::string("QUIT :") + reason + "\r\n")

#define ERR_UNKNOWNCOMMAND(nick, command)                "421 " + nick + " " + command + " :Unknown command\r\n"

#define ERR_NEEDMOREPARAMS(client, command)                "461 " + client + " " + command + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTERED(client)                    "462 " + client + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(client)                        "464 " + client + " :Password incorrect\r\n"

#define ERR_NICKNAMEINUSE(client, nick)                    "433 " + client + " " + nick + " :Nickname is already in use\r\n"

#define ERR_NONICKNAMEGIVEN(client)                        "431 " + client + " :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(client, nick)                "432 " + client + " " + nick + " :Erroneus nickname\r\n"

#define ERR_NOTREGISTERED(nick)                            "451 " + nick + " :You have not registered\r\n"

#define ERR_BADCHANNELKEY(nick, channel)                "475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"

#define ERR_BADCHANMASK(nick, channel)                    "476 " + nick + " " + channel + " :Bad Channel Mask\r\n"

#define ERR_CHANNELISFULL(nick, channel)                "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"

#define ERR_NOSUCHCHANNEL(nick, channel)                "403 " + nick + " " + channel + " :No such channel\r\n"

#define ERR_NOTONCHANNEL(nick, channel)                    "442 " + nick + " " + channel + " :You're not on that channel\r\n"

#define ERR_NOSUCHNICK(nick, target)                    "401 " + nick + " " + target + " :No such nick/channel\r\n"

#define ERR_USERSDONTMATCH(nick)                        "502 " + nick + " :Can't change mode for other users\r\n"

#define ERR_UMODEUNKNOWNFLAG(nick)                        "501 " + nick + " :Unknown MODE flag\r\n"

#define ERR_CHANOPRIVSNEEDED(nick, channel)                "482 " + nick + " " + channel + " :You're not channel operator\r\n"

#define ERR_INVITEONLYCHAN(nick, channel)                "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"

#define ERR_INVALIDMODEPARAM(nick, target, mode, arg, desc)    "696 " + nick + " " + target + " " + mode + " " + arg + " :" + desc + "\r\n"

#define ERR_UNKNOWNMODE(nick, mode)                        "472 " + nick + " " + mode + " :is unknown mode char to me\r\n"

#define ERR_USERONCHANNEL(nick, user, channel)            "443 " + nick + " " + user + " " + channel + " :is already on channel\r\n"

#define ERR_INVALIDKEY(nick, channel)                    "475 " + nick + " " + channel + " :Invalid channel key\r\n"

#define ERR_USERNOTINCHANNEL(nick, user, channel)        "441 " + nick + " " + user + " " + channel + " :They aren't on that channel\r\n"

#define ERR_NORECIPIENT(nick, command)                    "411 " + nick + " :No recipient given (" + command + ")\r\n"

#define ERR_NOTEXTTOSEND(nick)                            "412 " + nick + " :No text to send\r\n"

#define ERR_CANNOTSENDTOCHAN(nick, channel)                "404 " + nick + " " + channel + " :Cannot send to channel\r\n"

#define ERR_UNKNOWNERROR(nick, command, info)           "400 " + nick + " " + command + " :Unknown error " + info + "\r\n"

#endif
