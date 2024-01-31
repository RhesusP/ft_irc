/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/24 21:14:07 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

Message::Message(void)
{
	_raw = "";
	_source = "";
	_command = "";
	_response = ""; 
}

Message::Message(std::string const &raw)
{
	_raw = raw;
	std::cout << "[MESSAGE] new message with raw " << _raw << std::endl;
}

Message::~Message(void) {}

void Message::parser(void)
{
	size_t from = 0;
	if (_raw[0] == '@')
		setTags()
}


std::string const &Message::getRaw(void) const
{
	return _raw;
}

std::string const &Message::getResponse(void) const
{
	return _response;
}
