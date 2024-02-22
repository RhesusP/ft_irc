/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 19:15:00 by cbernot          ###   ########.fr       */
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
	_isRegistered = 0;
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
	_isRegistered = 0;
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

void User::setAuth(bool auth)
{
	_isAuth = auth;
}

int User::getIsRegistered(void) const
{
	return _isRegistered;
}

void User::setRegistered(int registered)
{
	_isRegistered = registered;
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


// User::User(void): _go_offline(false), _is_registered(false) {
// }

// User::User(int &sockfd, sockaddr_in &addr) : _sockfd(sockfd), _addr(addr), _is_registered(false), _go_offline(false) {}

// User::User(int &sockfd) : _sockfd(sockfd), _is_registered(false), _go_offline(false) {}

// User::~User(void) {}

// void User::setSocket(int val) {
// 	_sockfd = val;
// }


// sockaddr_in User::getAddress(void) const {
// 	return _addr;
// }

// int	User::getSocket(void) const {
// 	return _sockfd;
// }

// std::string User::getRealName(void) const {
// 	return _realname;
// }

// bool User::getRegistered(void) const {
// 	return _is_registered;
// }

// bool User::getStatus(void) const {
// 	return _go_offline;
// }

// std::time_t User::timeElapsed(void) const {
// 	return (std::time(NULL) - _timeval);
// }
