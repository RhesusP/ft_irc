/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:44:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/24 14:55:00 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "ircserv.hpp"

// Avoid 'use of undeclared identifier' error
class Channel;

class User
{
private:
	std::string _username;
	std::string _nickname;
	std::string _realname;
	int _sockfd; //clear fd when user disconnect
	sockaddr_in _addr;
	std::vector<Channel> _channels;

	bool _is_password_ok;
	bool _is_registered;

public:
	User(void);
	User(int &sockfd, sockaddr_in &addr);
	~User(void);

	sockaddr_in getAddress(void) const;
};

#endif
