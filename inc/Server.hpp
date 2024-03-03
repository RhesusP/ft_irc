/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 16:53:24 by cbernot          ###   ########.fr       */
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

class setSocketNonBlockingException : public std::exception {
	virtual const char* what() const throw();
};

class Server
{
private:
	int _servSocket;		// server socket
	
	int _port;
	std::string _password;
	std::string _name;
	std::vector<Channel> _channels;
	std::vector<User> _users;
	
	std::vector<struct pollfd> _clients_fds;				// clients by ufds
	std::time_t	_creation_time;
	// std::queue<Message> _waitingList;
	void formatRecv(std::string rec, User *usr);
	void setSocketNonBlocking(int fd);
	void waitingForClient(void);

public:
	socklen_t size;

	Server(void);
	Server(std::string port, std::string password);
	~Server(void);
	
	void initNetwork(void);
	void addUser(int socket, char *ip, int port);
	void removeUser(int socket);
	void readData(User *User);
	void acceptNewConnection(void);
	ssize_t sendData(std::string sender, std::string message, int fd);

	void setPassword(std::string const &port);
	void setPort(std::string const &port);
	int getPort(void) const;
	std::string const & getName(void) const;
	int getSocket(void) const;
	std::string getPassword(void) const;
	std::vector<User *> getUsers(void);
	time_t getCreationTime(void) const;
	std::vector<struct pollfd> getClientsFds(void);

	std::vector<Channel *> getChannels(void);
	Channel *getChannel(std::string const &name);
	Channel *addChannel(Channel channel);
	void removeChannel(Channel channel);
};

std::ostream & operator<<(std::ostream & o, Server & rhs);

#endif
