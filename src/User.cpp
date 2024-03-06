/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/06 10:28:25 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void) {
	_fd = -1;
	_port = -1;
	_username = "";
	_nickname = "";
	_realname = "";
	_hostname = "";
	_isAuth = false;
}

User::~User(void) {}

User::User(Server* server, int fd, std::string const & hostname, int port) {
	std::cout << "User " << fd << " | " << hostname << " created" << std::endl;
	_server = server;
	_fd = fd;
	_hostname = hostname;
	_port = port;
	_username = "";
	_nickname = "";
	_realname = "";
	_isAuth = false;
}

int User::getFD(void) const {
	return _fd;
}

std::string const & User::getNickname(void) const
{
	return _nickname;
}

bool User::getIsAuth(void) const
{
	return _isAuth;
}

void User::setIsAuth(bool auth)
{
	_isAuth = auth;
}

std::string const & User::getHostname(void) const
{
	return _hostname;
}

std::string const & User::getUsername(void) const
{
	return _username;
}

void User::setUsername(std::string const & username)
{
	_username = username;
}

void User::setNickname(std::string const & nickname)
{
	_nickname = nickname;
}

std::string const & User::getRealname(void) const
{
	return _realname;
}

void User::setRealname(std::string const & realname)
{
	_realname = realname;
}

std::string User::getIdentity(void) const
{
	std::string nick, user, host;

	nick = _nickname.empty() ? "*" : _nickname;
	user = _username.empty() ? "*" : _username;
	host = _hostname.empty() ? "*" : _hostname;
	return (nick + "!" + user + "@" + host);
}

bool User::isRegistered(void) const
{
	if (_nickname.size() > 0 && _username.size() > 0 && _realname.size() > 0)
		return true;
	return false;
}

void User::addinChannel(Channel *chan)
{
	_channels.push_back(chan);
}

void User::removefromChannel(Channel *chan)
{
	for (std::list<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == chan)
		{
			_channels.erase(it);
			break;
		}
	}
}

bool User::operator==(User const & rhs) const
{
	return (_fd == rhs.getFD() && _nickname == rhs.getNickname());
}

std::ostream & operator<<(std::ostream & o, User const & rhs)
{
	o << "------------------------------" << std::endl;
	o << "User (" << rhs.getFD() << ") " << std::endl;
	o << "\tNickname: " << rhs.getNickname() << std::endl;
	o << "\tUsername: " << rhs.getUsername() << std::endl;
	o << "\tRealname: " << rhs.getRealname() << std::endl;
	o << "\tHostname: " << rhs.getHostname() << std::endl;
	o << "\tIsAuth: " << (rhs.getIsAuth() ? "true" : "false") << std::endl;
	o << "------------------------------" << std::endl;
	return o;
}