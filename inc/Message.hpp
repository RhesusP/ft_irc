/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/02 14:58:53 by cbernot          ###   ########.fr       */
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
	PRIVMSG,

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
	command _command;
	std::vector<std::string> _parameters;
	std::string _response;

	std::string getTags(std::string const &raw);
	std::string getSource(std::string const &raw);
	std::string getCommand(std::string const &raw);
	void getParameters(std::string const & raw);

public:
	Message(void);
	Message(std::string const &raw);
	~Message(void);

	std::map<std::string, std::string> & getTags(void);
	std::string const & getSource(void);
	command	getCommand(void);
	std::vector<std::string> getParameters(void);
};

std::ostream &operator<<(std::ostream &o, Message &rhs);

#endif
