/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:58:08 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 12:44:35 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "ircserv.hpp"

std::vector<std::string> split(std::string str, std::string const &delimiter);

std::string trim(const std::string &str, std::string const &charset);

std::string timestr(std::time_t time);

std::string const &toupper(std::string &str);

void sendUserList(Server *server, User *user, Channel *channel, std::string const &chan_name);

bool isChanNameValid(std::string const &name);

bool is_nickname_valid(std::string const &nick);

#endif
