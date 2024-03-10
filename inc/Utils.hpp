/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:58:08 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 03:21:04 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "ircserv.hpp"

std::vector<std::string> split(std::string str, std::string delimiter);
std::string trim(const std::string & str, std::string charset);
std::vector<pollfd> lst_to_vec(std::list<pollfd> list);
std::string timestr(std::time_t time);
bool isUsernameValid(std::string username);

#endif