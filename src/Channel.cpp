/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:23 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/05 15:05:56 by cbernot          ###   ########.fr       */
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

Channel::Channel(Server *server, std::string const & name, User *founder)
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
	_operators.push_back(founder);
}

Channel::Channel(Server *server, std::string const & name, std::string const & key, User *founder)
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
	_operators.push_back(founder);
}

Channel::~Channel(void) {}

bool	Channel::isInChannel(User *user)
{
	if (isRegularMember(user) || isOperator(user))
		return true;
	return false;
}

bool	Channel::isRegularMember(User *user) const
{
	for (size_t i = 0 ; i < _members.size() ; i++)
	{
		if (_members[i] == user)
			return true;
	}
	return false;
}

bool	Channel::isOperator(User *user) const
{
	for (size_t i = 0 ; i < _operators.size() ; i++)
	{
		if (_operators[i] == user)
			return true;
	}
	return false;
}

bool Channel::addRegularMember(User *user)
{
	if (isInChannel(user))
	{
		// PRINT_ERROR("already in chan");
		return (false);
	}
	// PRINT_SUCCESS("NOT ALREADY IN CHAN");
	_members.push_back(user);
	user->addinChannel(this);
	return (true);
}

void Channel::removeRegularMember(User *user)
{
	for (size_t i = 0 ; i < _members.size() ; i++)
	{
		if (_members[i] == user)
		{
			PRINT_INFO("Removing " + user->getNickname() + " from channel " + _name);
			_members.erase(_members.begin() + i);
			user->removefromChannel(this);
			return ;
		}
	}
	if (_operators.size() == 0 && _members.size() == 0)
	{
		PRINT_INFO("No more members in channel " + _name + ", deleting it");
		_server->removeChannel(*this);
	}
}

void Channel::addOperator(User *user)
{
	// std::cout << "addOperator" << std::endl;
	// std::cout << "user: " << *user << std::endl;

	if (isRegularMember(user))
	{
		for (size_t i = 0 ; i < _members.size() ; i++)
		{
			if (_members[i] == user)
			{
				_members.erase(_members.begin() + i);
				break;
			}
		}
		_operators.push_back(user);
		PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
	}
	else
	{
		PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
		// std::cout << "user: " << user->getFD() << std::endl;
		_operators.push_back(user);
		user->addinChannel(this);
	}
}

void Channel::removeOperator(User *user)
{
	for (size_t i = 0 ; i < _operators.size() ; i++)
	{
		if (_operators[i] == user)
		{
			_operators.erase(_operators.begin() + i);
			user->removefromChannel(this);
			PRINT_INFO("Removing " + user->getNickname() + " from operators of channel " + _name);
			return ;
		}
	}
	if (_operators.size() == 0 && _members.size() > 0)
	{
		PRINT_INFO("No more operators in channel " + _name + ", setting " + _members[0]->getNickname() + " as operator");
		_operators.push_back(_members[0]);
	}
	else if (_operators.size() == 0 && _members.size() == 0)
	{
		PRINT_INFO("No more members in channel " + _name + ", deleting it");
		_server->removeChannel(*this);
	}
}

void Channel::removeUser(User *user)
{
	removeRegularMember(user);
	removeOperator(user);
}

std::vector<User*> Channel::getRegularMembers(void)
{
	return _members;
}

std::vector<User*> Channel::getOperators(void)
{
	return _operators;
}

size_t Channel::nbMembers(void) const
{
	return _members.size() + _operators.size();
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

void Channel::broadcast(User* sender, std::string const & message)
{
	for (size_t i = 0 ; i < _operators.size() ; i++)
	{
		if (_operators[i] != sender)
			_server->sendData(sender->getIdentity(), message,_operators[i]->getFD());
	}
	for (size_t i = 0 ; i < _members.size() ; i++)
	{
		if (_members[i] != sender)
			_server->sendData(sender->getIdentity(), message,_members[i]->getFD());
	}
}

void Channel::broadcast(Server *server, std::string const & message)
{
	for (size_t i = 0 ; i < _operators.size() ; i++)
	{
		server->sendData(server->getName(), message,_operators[i]->getFD());
	}
	for (size_t i = 0 ; i < _members.size() ; i++)
	{
		server->sendData(server->getName(), message,_members[i]->getFD());
	}
}

bool Channel::operator==(Channel const & rhs)
{
	return _name == rhs.getName();
}

Channel& Channel::operator=(Channel & rhs)
{
	_server = rhs._server;
	_members = rhs.getRegularMembers();
	_operators = rhs.getOperators();
	_name = rhs.getName();
	_topic = rhs.getTopic();
	_key = rhs.getKey();
	_limit = rhs.getLimit();
	_mode[0] = rhs.getModes()[0];
	_mode[1] = rhs.getModes()[1];
	_mode[2] = rhs.getModes()[2];
	_mode[3] = rhs.getModes()[3];
	_mode[4] = rhs.getModes()[4];
	return *this;
}

std::ostream & operator<<(std::ostream & o, Channel & rhs)
{
	o << "------------------------------------------" << std::endl;
	o << "Channel " << rhs.getName() << std::endl;
	o << "Topic: " << rhs.getTopic() << std::endl;
	o << "Key: " << rhs.getKey() << std::endl;
	o << "Limit: " << rhs.getLimit() << std::endl;
	o << "Mode: " << rhs.getModes()[0] << rhs.getModes()[1] << rhs.getModes()[2] << rhs.getModes()[3] << rhs.getModes()[4] << std::endl;
	o << "Members: " << std::endl;
	std::vector<User*> members = rhs.getRegularMembers();
	for (size_t i = 0 ; i < members.size() ; i++)
	{
		o << "\t" << "(" << members[i]->getFD() << ") " << members[i]->getNickname() << std::endl;
	}
	o << "Operators: " << std::endl;
	std::vector<User*> operators = rhs.getOperators();
	for (size_t i = 0 ; i < operators.size() ; i++)
	{
		o << "\t" << "(" << operators[i]->getFD() << ") " << operators[i]->getNickname() << std::endl;
	}
	o << "------------------------------------------" << std::endl;
	return o;
}
