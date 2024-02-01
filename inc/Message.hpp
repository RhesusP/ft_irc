/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/01 15:23:55 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"

enum command
{
	CAP,
	PASS,
	NICK,
	USER,
	PING,
	PONG,
	OPER,
	QUIT,

	KICK,
	INVITE,
	TOPIC,
	MODE,

	UNKNOWN
};

class BadTagException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class Message
{
private:
	std::string _raw;
	std::map<std::string, std::string> _tags;
	std::string _source;
	std::string _response;
	command _command;
	std::vector<std::string> _parameters;

	std::string getTags(std::string const &raw);
	std::string getSource(std::string const &raw);
	std::string getCommand(std::string const &raw);

public:
	Message(void);
	Message(std::string const &raw);
	~Message(void);
};

#endif
