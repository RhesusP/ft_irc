/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/19 16:26:07 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"

#define MAX_CON_QUEUE 20

// Avoid 'use of undeclared identifier' error
class User;
class Channel;

class Server
{
private:
	sockaddr_in _addr;
	int _socket;
	int _port;
	std::string _password;
	std::vector<Channel> _channels;
	std::vector<User> _users;
	std::vector<pollfd> _ufds;
	int _nfds;

public:
	Server(void);
	Server(std::string port, std::string password);
	~Server(void);
	void init_network(void);
	void setPort(std::string const &port);
	void setPassword(std::string const &port);
	void initpoll(void);
	std::string getPassword(void) const;
	int getPort(void) const;
	std::vector<Channel> getChannels(void);
	int	 runPoll(void);
	void handlePoll(void);
	void acceptNewConnection(void); //TODO: store client info in a User object
	void readData(int i); // TODO: data sent to server by client, so it's the main part of the server
};

#endif
