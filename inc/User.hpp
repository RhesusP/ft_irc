/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:44:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 11:17:36 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "ircserv.hpp"

// Avoid 'use of undeclared identifier' error
class Channel;
class Server;

class User
{
private:
	int	_fd;
	int _port;
	std::string _username;		// set by USER command
	std::string _realname;		// set by USER command
	std::string _nickname;		// set by NICK command
	std::string _hostname;
	bool _isAuth;
	int _isRegistered;			// Must be 2 to be fully registered
	std::vector<Channel> _channels;
	Server*	_server;
	std::string _msgStash;

public:
	User(void);
	User(Server* server, int fd, std::string const & hostname, int port);
	~User(void);

	std::string const & getNickname(void) const;
	std::string const & getUsername(void) const;
	std::string const & getRealname(void) const;
	std::string const & getHostname(void) const;

	void setNickname(std::string const & nickname);
	void setUsername(std::string const & username);
	void setRealname(std::string const & realname);

	int getFD(void) const;
	bool getIsAuth(void) const;
	void setAuth(bool auth);
	int getIsRegistered(void) const;
	void setRegistered(int registered);

};

#endif
