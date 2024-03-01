/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:44:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 11:46:20 by cbernot          ###   ########.fr       */
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
	std::string _hostname;		// set during connection process
	bool _isAuth;
	std::vector<Channel> _channels;
	Server*	_server;
	std::string _msgStash;		// TODO implement if we lost a message while reading from several sockets

public:
	User(void);
	User(Server* server, int fd, std::string const & hostname, int port);
	~User(void);

	std::string const & getNickname(void) const;
	std::string const & getUsername(void) const;
	std::string const & getRealname(void) const;
	std::string const & getHostname(void) const;
	std::string getIdentity(void) const;

	void setNickname(std::string const & nickname);
	void setUsername(std::string const & username);
	void setRealname(std::string const & realname);

	int getFD(void) const;
	bool getIsAuth(void) const;
	void setAuth(bool auth);
};

#endif