/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/08 11:38:54 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void): _go_offline(false), _is_registered(false), _is_password_ok(false) {
}

User::User(int &sockfd, sockaddr_in &addr) {
	_sockfd = sockfd;
	_addr = addr;
	_is_registered = false;
	_is_password_ok = false;
}

User::User(int &sockfd) : _sockfd(sockfd), _is_registered(false), _is_password_ok(false), _go_offline(false) {}

User::~User(void) {}

void User::setSocket(int val) {
	_sockfd = val;
}


sockaddr_in User::getAddress(void) const {
	return _addr;
}

int	User::getSocket(void) const {
	return _sockfd;
}

std::string User::getRealName(void) const {
	return _realname;
}

bool User::getRegistered(void) const {
	return _is_registered;
}

bool User::getStatus(void) const {
	return _go_offline;
}

void	User::offline(void) {
	_sockfd = -1;
}
