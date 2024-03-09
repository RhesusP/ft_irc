/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:23 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/09 20:27:26 by cbernot          ###   ########.fr       */
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
	_is_invite_only = false;
	_is_topic_restricted = false;
	_creation_time = time(NULL);
}

Channel::Channel(Server *server, std::string const & name, User *founder)
{
	_server = server;
	_name = name;
	_topic = "";
	_key = "";
	_limit = -1;	// no limit
	_is_invite_only = false;
	_is_topic_restricted = false;
	_operators.push_back(founder);
	_creation_time = time(NULL);
}

Channel::Channel(Server *server, std::string const & name, std::string const & key, User *founder)
{
	_server = server;
	_name = name;
	_topic = "";
	_key = key;
	_limit = -1;	// no limit
	_is_invite_only = false;
	_is_topic_restricted = false;
	_operators.push_back(founder);
	_creation_time = time(NULL);
}

Channel::~Channel(void) {}

bool	Channel::isInChannel(User *user)
{
	if (isRegularMember(user) || isOperator(user))
		return true;
	return false;
}

bool	Channel::isRegularMember(User *user)
{
	for (std::list<User*>::iterator it = _members.begin() ; it != _members.end() ; it++)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool	Channel::isOperator(User *user)
{
	for (std::list<User*>::iterator it = _operators.begin() ; it != _operators.end() ; it++)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool	Channel::isOperator(std::string const & nickname)
{
	for (std::list<User*>::iterator it = _operators.begin() ; it != _operators.end() ; it++)
	{
		if ((*it)->getNickname() == nickname)
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
	for (std::list<User*>::iterator it = _members.begin() ; it != _members.end() ; it++)
	{
		if (*it == user)
		{
			PRINT_INFO("Removing " + user->getNickname() + " from channel " + _name);
			_members.erase(it);
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
	if (isRegularMember(user))
	{
		for (std::list<User*>::iterator it = _members.begin() ; it != _members.end() ; it++)
		{
			if (*it == user)
			{
				_members.erase(it);
				break;
			}
		}
		_operators.push_back(user);
		PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
	}
	else
	{
		PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
		_operators.push_back(user);
		user->addinChannel(this);
	}
}

void Channel::removeOperator(User *user)
{
	PRINT_INFO("Removing " + user->getNickname() + " from operators of channel " + _name);
	for (std::list<User*>::iterator it = _operators.begin() ; it != _operators.end() ; it++)
	{
		if (*it == user)
		{
			PRINT_INFO("Removing " + user->getNickname() + " from channel " + _name);
			_operators.erase(it);
			user->removefromChannel(this);
			break;
		}
	}
	if (_operators.size() == 0 && _members.size() > 0)
	{
		User *new_operator = *_members.begin();
		_operators.push_back(new_operator);
		_members.erase(_members.begin());
		PRINT_INFO("No more operators in channel " + _name + ", setting " + new_operator->getNickname() + " as operator");
	}

	else if (_operators.size() == 0 && _members.size() == 0)
	{
		PRINT_INFO("No more members in channel " + _name + ", deleting it");
		_server->removeChannel(*this);
	}
}

void Channel::removeUser(User *user)
{
	removeOperator(user);
	removeRegularMember(user);
}

std::list<User*> Channel::getRegularMembers(void)
{
	return _members;
}

std::list<User*> Channel::getOperators(void)
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

bool Channel::isInviteOnly(void) const
{
	return _is_invite_only;
}

bool Channel::isTopicRestricted(void) const
{
	return _is_topic_restricted;
}

std::string Channel::getModes(void) const
{
	std::string res = "";
	if (_limit != -1)
		res += "+l";
	if (_is_invite_only)
		res += "i";
	if (_is_topic_restricted)
		res += "t";
	if (_key.size() > 0)
		res += "k";
	return res;
}

std::time_t Channel::getCreationTime(void) const
{
	return _creation_time;
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
void Channel::setInviteOnly(bool is_invite_only)
{
	_is_invite_only = is_invite_only;
}

void Channel::setTopicRestricted(bool is_topic_restricted)
{
	_is_topic_restricted = is_topic_restricted;
}

void Channel::addMode(char mode, std::string const & arg, User *sender)
{
	// std::cout << "Adding mode " << mode << " with arg " << arg << std::endl;
	int limit;
	User *target;
	std::list<User *> lst;
	switch (mode)
	{
	case 'l':
		limit = atoi(arg.c_str());
		if (arg.size() > 10 || limit < 1)
		{
			_server->sendData(_server->getName(), ERR_INVALIDMODEPARAM(sender->getNickname(), this->getName(), "MODE +l", arg, "limit is not valid"), sender->getFD());
			return ;
		}
		this->setLimit(limit);
		this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+l " + arg));
		_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+l " + arg), sender->getFD());
		break;
	case 't':
		this->setTopicRestricted(true);
		this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+t"));
		_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+t"), sender->getFD());
		break;
	case 'k':
		if (arg.size() > 0)
		{
			this->setKey(arg);
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+k"));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+k"), sender->getFD());
		}
		else
		{
			_server->sendData(_server->getName(), ERR_INVALIDMODEPARAM(sender->getNickname(), this->getName(), "MODE +k", "", "key is missing"), sender->getFD());
			return ;
		}
		break;
	case 'o':
		lst = this->getRegularMembers();
		for (std::list<User *>::iterator it = lst.begin() ; it != lst.end() ; it++)
		{
			if ((*it)->getNickname() == arg)
			{
				target = *it;
				break;
			}
		}
		if (target)
		{
			this->addOperator(target);
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+o " + arg));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+o " + arg), sender->getFD());
		}
		else
		{
			if (!this->isOperator(arg))
				_server->sendData(_server->getName(), ERR_NOTONCHANNEL(sender->getNickname(), this->getName()), sender->getFD());
		}
		break;
	case 'i':
		this->setInviteOnly(true);
		this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+i"));
		_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+i"), sender->getFD());
		break;
	default:
		_server->sendData(_server->getName(), ERR_UNKNOWNMODE(sender->getNickname(), mode), sender->getFD());
		break;
	}
}

void Channel::removeMode(char mode, std::string const & arg, User *sender)
{
	std::cout << "Removing mode " << mode << " with arg " << arg << std::endl;
	switch (mode)
	{
	case 'l':
		if (this->getLimit() != -1)
		{
			this->setLimit(-1);
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-l"));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-l"), sender->getFD());
		}
		break;
	case 't':
		if (this->isTopicRestricted())
		{
			this->setTopicRestricted(false);
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-t"));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-t"), sender->getFD());
		}
		break;
	case 'k':
		if (this->getKey().size() > 0)
		{
			this->setKey("");
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-k"));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-k"), sender->getFD());
		}
		break;
	case 'o':
		if (this->isOperator(arg))
		{
			this->removeOperator(_server->getUser(arg));
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-o " + arg));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-o " + arg), sender->getFD());
		}
		else
		{
			if (!this->isRegularMember(_server->getUser(arg)))
				_server->sendData(_server->getName(), ERR_NOTONCHANNEL(sender->getNickname(), this->getName()), sender->getFD());
		}
		break;
	case 'i':
		if (this->isInviteOnly())
		{
			this->setInviteOnly(false);
			this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-i"));
			_server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-i"), sender->getFD());
		}
		break;
	default:
		_server->sendData(_server->getName(), ERR_UNKNOWNMODE(sender->getNickname(), mode), sender->getFD());
		break;
	}
}

void Channel::broadcast(User* sender, std::string const & message)
{
	for (std::list<User*>::iterator it = _members.begin() ; it != _members.end() ; it++)
	{
		if (*it != sender)
			_server->sendData(sender->getIdentity(), message,(*it)->getFD());
	}
	for (std::list<User*>::iterator it = _operators.begin() ; it != _operators.end() ; it++)
	{
		if (*it != sender)
			_server->sendData(sender->getIdentity(), message,(*it)->getFD());
	}
}

void Channel::broadcast(Server *server, std::string const & message)
{
	for (std::list<User*>::iterator it = _members.begin() ; it != _members.end() ; it++)
	{
		_server->sendData(server->getName(), message,(*it)->getFD());
	}
	for (std::list<User*>::iterator it = _operators.begin() ; it != _operators.end() ; it++)
	{
		_server->sendData(server->getName(), message,(*it)->getFD());
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
	_is_invite_only = rhs.isInviteOnly();

	return *this;
}

std::ostream & operator<<(std::ostream & o, Channel & rhs)
{
	o << "------------------------------------------" << std::endl;
	o << "Channel " << rhs.getName() << std::endl;
	o << "Topic: " << rhs.getTopic() << std::endl;
	o << "Key: " << rhs.getKey() << std::endl;
	o << "Limit: " << rhs.getLimit() << std::endl;
	o << "Mode: " << rhs.getModes() << std::endl;
	o << "Members: " << std::endl;
	std::list<User*> members = rhs.getRegularMembers();
	for (std::list<User*>::iterator it = members.begin() ; it != members.end() ; it++)
	{
		o << "\t" << "(" << (*it)->getFD() << ") " << (*it)->getNickname() << std::endl;
	}
	o << "Operators: " << std::endl;
	std::list<User*> operators = rhs.getOperators();
	for (std::list<User*>::iterator it = operators.begin() ; it != operators.end() ; it++)
	{
		o << "\t" << "(" << (*it)->getFD() << ") " << (*it)->getNickname() << std::endl;
	}
	o << "------------------------------------------" << std::endl;
	return o;
}
