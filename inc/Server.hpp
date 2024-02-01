/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/31 23:27:16 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"

#define MAX_CON_QUEUE 20

// Avoid 'use of undeclared identifier' error
class User;
class Channel;
class Message;

class Server
{
private:
	int _port;
	std::string _password;
	std::vector<Channel> _channels;
	std::queue<Message> _waitingList;
	void formatRecv(char *buf);

public:
	Server(void);
	Server(std::string port, std::string password);
	~Server(void);
	void init_network(void);
	void setPort(std::string const &port);
	void setPassword(std::string const &port);
	std::string getPassword(void) const;
	int getPort(void) const;
	std::queue<Message> getWaitingList(void);
	std::vector<Channel> getChannels(void);
};

#endif