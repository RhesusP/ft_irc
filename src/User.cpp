/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/02 17:57:44 by cbernot          ###   ########.fr       */
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

bool User::operator==(User const & rhs) const
{
	return (_fd == rhs.getFD() && _nickname == rhs.getNickname());
}
