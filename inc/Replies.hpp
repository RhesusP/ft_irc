/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:05:15 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 13:16:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define RPL_WELCOME(client, nick, username, hostname)		"001 " + client + " :Welcome to the irc_serv Network, " + nick + "!" + username + "@" + hostname + "\r\n"

#define RPL_YOURHOST(client)								"002 " + client + " :Your host is irc_serv, running version 1\r\n"

#define RPL_CREATED(client, datetime)						"003 " + client + " :This server was created " + datetime + "\r\n"

#define RPL_LUSERCLIENT(user_nb)							"251 :There are " + user_nb + " users on 1 server\r\n"

#define RPL_LUSERME(cli_nb)									"255 :I have " + cli_nb + " clients and 1 server\r\n"

#define RPL_MOTDSTART(client)								"375 " + client + " :- irc_serv Message of the Day - \r\n"

#define RPL_MOTD(client, line)								"372 " + client + " : " + line + "\r\n"

#define RPL_ENDOFMOTD(client)								"376 " + client + " :End of /MOTD command\r\n"

#define ERR_UNKNOWNCOMMAND(client, command) "421 " + client + " " + command + " :Unknown command\r\n"

#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect\r\n"

#define ERR_NICKNAMEINUSE(client, nick) "433 " + client + " " + nick + " :Nickname is already in use\r\n"

#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(client, nick) "432 " + client + " " + nick + " :Erroneus nickname\r\n"

#endif