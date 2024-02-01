/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/31 23:27:42 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

const char *BadTagException::what() const throw()
{
	return "Bad tag";
}

Message::Message(void)
{
	_raw = "";
	
}

void Message::getTags(std::string const & raw)
{
	if (raw[0] != '@')
		return ;
	std::string str(&raw[1]);
	std::vector<std::string> tags_tab = split(str, ';');
	std::vector<std::string> tag_tab;
	for (std::vector<std::string>::iterator it = tags_tab.begin(); it != tags_tab.end() ; it++)
	{
		tag_tab = split(*it, '=');
		if (tag_tab.size() != 2)
			throw BadTagException();
		std::vector<std::string>::iterator ita = tag_tab.begin();
		std::string key = *ita;
		++ita;
		std::string value = *ita;
		_tags.insert({key, value});
		tag_tab.clear();
	}
}

Message::Message(std::string const &raw)
{
	_raw = raw;
	std::cout << "[MESSAGE] new message with raw " << _raw << std::endl;
	try
	{
		getTags(raw);
	}
	catch (std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

Message::~Message(void) {}
