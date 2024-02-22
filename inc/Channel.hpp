/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:43:33 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 14:27:46 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ircserv.hpp"

// Avoid 'use of undeclared identifier' error
class User;

class Channel
{
private:
	std::vector<User> _members;
	std::vector<User> _operators;
	std::string _name;
	std::string _topic;
	std::string _key;	// password
	int _limit;
	char _mode[5];		// i t k o l

public:
	Channel(void);
	~Channel(void);

	void addMember(User &user);
	void removeMember(User &user);
	void addOperator(User &user);
	void removeOperator(User &user);

	std::vector<User> & getMembers(void);
	std::vector<User> & getOperators(void);
};

#endif