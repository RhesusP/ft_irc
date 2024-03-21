/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:40:51 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 12:44:17 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

/**
 * @brief Split a string using a string as delimiter
 *
 * @param str String to split
 * @param delimiter Delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(std::string str, std::string delimiter)
{
	std::vector<std::string> res;
	size_t pos = 0;
	size_t del_len = delimiter.size();
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		res.push_back(str.substr(0, pos));
		str.erase(0, pos + del_len);
	}
	if (!str.empty())
	{
		res.push_back(str);
	}
	return res;
}

/**
 * @brief Trim a string from a given charset
 *
 * @param str
 * @param charset
 * @return std::string Trimmed string
 */
std::string trim(const std::string &str, std::string charset)
{
	size_t first = str.find_first_not_of(charset);
	if (first == std::string::npos)
	{
		return str;
	}
	size_t last = str.find_last_not_of(charset);
	return str.substr(first, (last - first + 1));
}

/**
 * @brief Convert a time_t to a string in the format "YYYY-MM-DDTHH:MM:SS.Z"
 *
 * @param time
 * @return std::string
 */
std::string timestr(std::time_t time)
{
	struct tm *timeinfo = gmtime(&time);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%Z", timeinfo);
	return std::string(buffer);
}

std::string const & toupper(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		str[i] = std::toupper(str[i]);
	}
	return str;
}

void sendUserList(Server *server, User *user, Channel *channel, std::string chan_name)
{
	if (!server->getChannel(chan_name))
		return ;
	std::string serv_name = server->getName();
	std::list<User *> members = channel->getRegularMembers();
	std::list<User *> chops = channel->getOperators();
	for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++)
	{
		channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), "@" + (*it)->getNickname()));
	}
	for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++)
	{
		channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), "@" + (*it)->getNickname()));
	}
	channel->broadcast(server, RPL_ENDOFNAMES(user->getNickname(), channel->getName()));
}
