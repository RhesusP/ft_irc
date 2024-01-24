/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/24 10:09:56 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void) {
	_is_registered = false;
	_is_password_ok = false;
}

User::User(int &sockfd, sockaddr_in &addr) {
	_sockfd = sockfd;
	_addr = addr;
	_is_registered = false;
	_is_password_ok = false;
}

User::~User(void) {}

sockaddr_in User::getAddress(void) const {
	return _addr;
}
