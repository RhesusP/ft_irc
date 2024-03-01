/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:40:51 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 19:13:33 by cbernot          ###   ########.fr       */
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
	int pos = 0;
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