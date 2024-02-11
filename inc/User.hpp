/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:44:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/11 16:19:41 by svanmeen         ###   ########.fr       */
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
	int _sockfd;
	sockaddr_in _addr;
	std::vector<Channel> _channels;

	std::time_t	_timeval;

	bool _is_registered;

	bool _go_offline;

public:
	User(void);
	User(int &sockfd, sockaddr_in &addr);
	User(int &sockfd);
	~User(void);

	void setSocket(int val);

	sockaddr_in getAddress(void) const;
	int	getSocket(void) const;
	std::string getRealName(void) const;
	bool getRegistered(void) const;
	bool getStatus(void) const;
	std::time_t timeElapsed(void) const;
};

#endif
