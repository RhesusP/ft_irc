/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:40:51 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/08 10:09:50 by cbernot          ###   ########.fr       */
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
std::vector<std::string>	split(std::string str, std::string delimiter)
{
	std::vector<std::string> res;
	unsigned int pos = 0;
	int del_len = delimiter.size();
	while (pos < str.size())
	{
		pos = str.find(delimiter);
		res.push_back(str.substr(0, pos));
		str.erase(0, pos + del_len);		
	}
	if (str.size() > 0 && str != res.back())
	{
		res.push_back(str);
	}
	return res;
}

std::string trim(const std::string & str, std::string charset)
{
	size_t first = str.find_first_not_of(charset);
	if (first == std::string::npos)
	{
		return str;
	}
	size_t last = str.find_last_not_of(charset);
	return str.substr(first, (last - first + 1));
}


std::vector<pollfd> lst_to_vec(std::list<pollfd> list)
{
	std::vector<pollfd> vec;
	for (std::list<pollfd>::iterator it = list.begin(); it != list.end(); it++)
	{
		vec.push_back(*it);
	}
	return vec;
}

std::string timestr(std::time_t time)
{
	struct tm *timeinfo = gmtime(&time);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%Z", timeinfo);
	return std::string(buffer);
}
