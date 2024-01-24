/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/22 18:20:03 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"

class Message
{
private:
	std::string _raw;
	std::string _content;
	std::string _response;

public:
	Message(void);
	Message(std::string const & raw);
	~Message(void);
	std::string const &getRaw(void) const;
	std::string const &getContent(void) const;
	std::string const &getResponse(void) const;
};

#endif