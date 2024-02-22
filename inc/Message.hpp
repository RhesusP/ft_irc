/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 17:47:38 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"
#include "User.hpp"

class Server;
// class User;

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
	std::string _command;
	User *_author;
	std::vector<std::string> _parameters;
	std::string _response;
	Server*	_server;




	std::string getTags(std::string const &raw);
	std::string getSource(std::string const &raw);
	std::string getCommand(std::string const &raw);
	void getParameters(std::string const & raw);

public:
	Message(void);
	Message(Server *server, User *user, std::string const &raw);
	~Message(void);

	std::map<std::string, std::string> & getTags(void);
	std::string const & getSource(void);
	std::string const &	getCommand(void) const;
	std::vector<std::string> getParameters(void);
	void	processMessage(void);
};

std::ostream &operator<<(std::ostream &o, Message &rhs);

#endif
