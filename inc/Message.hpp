/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 22:51:00 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"
#include "User.hpp"

class Server;

class Message
{
private:
	User *_author;
	Server *_server;
	std::string _raw;
	std::map<std::string, std::string> _tags;
	std::string _source;
	std::string _command;
	std::vector<std::string> _parameters;
	std::string getTags(std::string const &raw);
	std::string getSource(std::string const &raw);
	std::string getCommand(std::string const &raw);
	void getParameters(std::string const &raw);
	void processMessage(void);

public:
	Message(void);
	Message(Server *server, User *user, std::string const &raw);
	~Message(void);

	// Getters
	std::map<std::string, std::string> &getTags(void);
	std::string const &getSource(void);
	std::string const &getCommand(void) const;
	std::vector<std::string> getParameters(void);
	User *getAuthor(void) const;
};

std::ostream &operator<<(std::ostream &o, Message &rhs);

#endif
