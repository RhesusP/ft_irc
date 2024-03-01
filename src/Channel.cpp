/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:23 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 18:03:38 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(Server *server)
{
	_server = server;
	_name = "";
	_topic = "";
	_key = "";
	_limit = -1;	// no limit
	_mode[0] = 0;	// i
	_mode[1] = 0;	// t
	_mode[2] = 0;	// k
	_mode[3] = 0;	// o
	_mode[4] = 0;	// l
}

Channel::Channel(Server *server, std::string const & name, User &founder)
{
	_server = server;
	_name = name;
	_topic = "";
	_key = "";
	_limit = -1;	// no limit
	_mode[0] = 0;	// i
	_mode[1] = 0;	// t
	_mode[2] = 0;	// k
	_mode[3] = 0;	// o
	_mode[4] = 0;	// l
	_members.push_back(founder);
	_operators.push_back(founder);
}

Channel::Channel(Server *server, std::string const & name, std::string const & key, User &founder)
{
	_server = server;
	_name = name;
	_topic = "";
	_key = key;
	_limit = -1;	// no limit
	_mode[0] = 0;	// i
	_mode[1] = 0;	// t
	_mode[2] = 0;	// k
	_mode[3] = 0;	// o
	_mode[4] = 0;	// l
	_members.push_back(founder);
	_operators.push_back(founder);
}

Channel::~Channel(void) {}

void Channel::addMember(User &user)
{
	PRINT_INFO("Adding " + user.getNickname() + " to channel " + _name);
	_members.push_back(user);
}

void Channel::removeMember(User &user)
{
	for (size_t i = 0 ; i < _members.size() ; i++)
	{
		if (_members[i] == user)
		{
			PRINT_INFO("Removing " + user.getNickname() + " from channel " + _name);
			_members.erase(_members.begin() + i);
			return ;
		}
	}
	if (_operators.size() == 0 && _members.size() == 0)
	{
		PRINT_INFO("No more members in channel " + _name + ", deleting it");
		_server->removeChannel(*this);
	}
}

void Channel::addOperator(User &user)
{
	PRINT_INFO("Adding " + user.getNickname() + " to operators of channel " + _name);
	_operators.push_back(user);
}

void Channel::removeOperator(User &user)
{
	for (size_t i = 0 ; i < _operators.size() ; i++)
	{
		if (_operators[i] == user)
		{
			_operators.erase(_operators.begin() + i);
			PRINT_INFO("Removing " + user.getNickname() + " from operators of channel " + _name);
			return ;
		}
	}
	if (_operators.size() == 0 && _members.size() > 0)
	{
		PRINT_INFO("No more operators in channel " + _name + ", setting " + _members[0].getNickname() + " as operator");
		_operators.push_back(_members[0]);
	}
	else if (_operators.size() == 0 && _members.size() == 0)
	{
		PRINT_INFO("No more members in channel " + _name + ", deleting it");
		_server->removeChannel(*this);
	}
}

std::vector<User> Channel::getMembers(void) const
{
	return _members;
}

std::vector<User> Channel::getOperators(void) const
{
	return _operators;
}

std::string const & Channel::getName(void) const
{
	return _name;
}

std::string const & Channel::getTopic(void) const
{
	return _topic;
}

std::string const & Channel::getKey(void) const
{
	return _key;
}

int Channel::getLimit(void) const
{
	return _limit;
}

int const * Channel::getModes(void) const
{
	return _mode;
}

void Channel::setTopic(std::string const & topic)
{
	_topic = topic;
}

void Channel::setKey(std::string const & key)
{
	_key = key;
}

void Channel::setLimit(int limit)
{
	_limit = limit;
}

bool Channel::operator==(Channel const & rhs) const
{
	return _name == rhs.getName();
}
