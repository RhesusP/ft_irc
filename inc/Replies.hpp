/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:05:15 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/08 11:39:37 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define RPL_WELCOME(nick, hostname, client_identity)	"001 " + nick + " :Welcome to the " + hostname + " Network, " + client_identity + "\r\n"

#define RPL_YOURHOST(nick, hostname)					"002 " + nick + " :Your host is " + hostname + ", running version 1\r\n"

#define RPL_CREATED(nick, datetime)						"003 " + nick + " :This server was created " + datetime + "\r\n"

#define RPL_LUSERCLIENT(nick, user_nb)					"251 " + nick + " :There are " + user_nb + " users on 1 server\r\n"

#define RPL_LUSERME(nick, cli_nb)						"255 " + nick + " :I have " + cli_nb + " clients and 1 server\r\n"

#define RPL_MOTDSTART(nick)								"375 " + nick + " :irc_serv Message of the Day - \r\n"

#define RPL_MOTD(nick, line)							"372 " + nick + " :" + line + "\r\n"

#define RPL_ENDOFMOTD(nick)								"376 " + nick + " :End of Message of the Day\r\n"

#define RPL_TOPIC(nick, channel, topic)					"332 " + nick + " " + channel + " :" + topic + "\r\n"

#define RPL_NAMREPLY(nick, channel, name)				"353 " + nick + " = " + channel + " :" + name + "\r\n"

#define RPL_ENDOFNAMES(nick, channel)					"366 " + nick + " " + channel + " :End of /NAMES list\r\n"

#define RPL_UMODEIS(nick, mode)							"221 " + nick + " " + mode + "\r\n"

#define RPL_CHANNELMODEIS(nick, channel, mode)			"324 " + nick + " " + channel + " " + mode + "\r\n"

#define RPL_CREATIONTIME(nick, channel, time)			"329 " + nick + " " + channel + " " + time + "\r\n"

#define ERR_UNKNOWNCOMMAND(nick, command) 				"421 " + nick + " " + command + " :Unknown command\r\n"

#define ERR_NEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTERED(client)					"462 " + client + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(client)						"464 " + client + " :Password incorrect\r\n"

#define ERR_NICKNAMEINUSE(client, nick)					"433 " + client + " " + nick + " :Nickname is already in use\r\n"

#define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(client, nick)				"432 " + client + " " + nick + " :Erroneus nickname\r\n"

#define ERR_NOTREGISTERED(nick)							"451 " + nick + " :You have not registered\r\n"

#define ERR_BADCHANNELKEY(nick, channel)				"475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"

#define ERR_BADCHANMASK(nick, channel)					"476 " + nick + " " + channel + " :Bad Channel Mask\r\n"

#define ERR_CHANNELISFULL(nick, channel)				"471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"

#define ERR_NOSUCHCHANNEL(nick, channel)				"403 " + nick + " " + channel + " :No such channel\r\n"

#define ERR_NOTONCHANNEL(nick, channel)					"442 " + nick + " " + channel + " :You're not on that channel\r\n"

#define ERR_NOSUCHNICK(nick, target)					"401 " + nick + " " + target + " :No such nick/channel\r\n"

#define ERR_USERSDONTMATCH(nick)						"502 " + nick + " :Can't change mode for other users\r\n"

#define ERR_UMODEUNKNOWNFLAG(nick)						"501 " + nick + " :Unknown MODE flag\r\n"

#define ERR_CHANOPRIVSNEEDED(nick, channel)				"482 " + nick + " " + channel + " :You're not channel operator\r\n"

#endif