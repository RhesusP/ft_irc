/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/15 13:46:00 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"

class Server;

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
	Server	*_server;
	std::string _raw;
	std::map<std::string, std::string> _tags;
	std::string _source;
	command _command;
	std::vector<std::string> _parameters;
	std::string _response;
	User	*_author;

	// author
	// array of recipients

	std::string getTags(std::string const &raw);
	std::string getSource(std::string const &raw);
	std::string getCommand(std::string const &raw);
	void getParameters(std::string const & raw);

public:
	Message(void);
	Message(std::string const &raw, Server * server, User *author);
	~Message(void);

	std::map<std::string, std::string> & getTags(void);
	std::string const & getSource(void);
	command	getCommand(void);
	std::vector<std::string> getParameters(void);
};

std::ostream &operator<<(std::ostream &o, Message &rhs);

#endif
