/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:43:33 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/06 10:37:16 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ircserv.hpp"

// Avoid 'use of undeclared identifier' error
class User;
class Server;

class Channel
{
private:
	Server *_server;
	std::list<User*> _members;
	std::list<User*> _operators;
	std::string _name;
	std::string _topic;
	std::string _key;	// password
	int _limit;
	int _mode[5];		// i t k o l

public:
	Channel(Server *server);
	Channel(Server *server, std::string const & name, User *founder);
	Channel(Server *server, std::string const & name, std::string const & key, User *founder);
	~Channel(void);

	bool addRegularMember(User *user);
	void removeRegularMember(User *user);

	void removeUser(User *user);
	
	void addOperator(User *user);
	void removeOperator(User *user);

	bool isInChannel(User *user);
	bool isRegularMember(User *user);
	bool isOperator(User *user);

	std::list<User*> getRegularMembers(void);
	std::list<User*> getOperators(void);
	size_t nbMembers(void) const;
	std::string const & getName(void) const;
	std::string const & getTopic(void) const;
	std::string const & getKey(void) const;
	int getLimit(void) const;
	int const * getModes(void) const;

	void setTopic(std::string const & topic);
	void setKey(std::string const & key);
	void setLimit(int limit);
	// void setMode();

	// TODO check if it is the best solution
	// bool isInviteOnly(void) const;

	void broadcast(User *sender, std::string const & message);
	void broadcast(Server *server, std::string const & message);

	bool operator==(Channel const & rhs);
	Channel& operator=(Channel & rhs);
};

std::ostream & operator<<(std::ostream & o, Channel & rhs);

#endif