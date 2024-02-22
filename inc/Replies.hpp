/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:05:15 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 18:06:33 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define RPL_WELCOME(source, nickname, username, hostname)		"001 " + source + " :Welcome to the irc_serv Network, " + nickname + "!" + username + "@" + hostname + "\r\n"

#define ERR_UNKNOWNCOMMAND(client, command) "421 " + client + " " + command + " :Unknown command\r\n"

#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect\r\n"

#endif