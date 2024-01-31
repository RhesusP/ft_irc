/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:58:08 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/31 14:08:44 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "ircserv.hpp"

/**
 * @brief Split a string using a char or a string as delimiter
 * 
 * @tparam T (char | std::string)
 * @param str string to split
 * @param delimiter delimiter
 * @return std::vector<std::string> 
 */
template <typename T>
std::vector<std::string>	split(std::string str, T delimiter)
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
	return res;
}

#endif