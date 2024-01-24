/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/22 18:22:20 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

Message::Message(void)
{
	_raw = "";
	_content = "";
}

Message::Message(std::string const &raw)
{
	_raw = raw;
	std::cout << "[MESSAGE]: new message with raw '" << _raw << "'" << std::endl;
}

Message::~Message(void) {}

std::string const &Message::getContent(void) const
{
	return _content;
}

std::string const &Message::getRaw(void) const
{
	return _raw;
}

std::string const &Message::getResponse(void) const
{
	return _response;
}
