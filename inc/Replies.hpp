/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:05:15 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 12:06:23 by cbernot          ###   ########.fr       */
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

#define ERR_UNKNOWNCOMMAND(nick, command) 				"421 " + nick + " " + command + " :Unknown command\r\n"

#define ERR_NEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTERED(client)					"462 " + client + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(client)						"464 " + client + " :Password incorrect\r\n"

#define ERR_NICKNAMEINUSE(client, nick)					"433 " + client + " " + nick + " :Nickname is already in use\r\n"

#define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(client, nick)				"432 " + client + " " + nick + " :Erroneus nickname\r\n"

#define ERR_NOTREGISTERED(nick)							"451 " + nick + " :You have not registered\r\n"

#endif