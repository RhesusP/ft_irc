/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/29 19:43:03 by cbernot          ###   ########.fr       */
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
	void formatRecv(std::string rec, User &usr);
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
	void readData(User &User);
	void acceptNewConnection(void);
	ssize_t sendData(std::string message, int fd);

	void setPassword(std::string const &port);
	void setPort(std::string const &port);
	int getPort(void) const;
	std::string const & getName(void) const;
	int getSocket(void) const;
	std::string getPassword(void) const;
	std::vector<User> getUsers(void) const;
	time_t getCreationTime(void) const;
	std::vector<struct pollfd> getClientsFds(void) const;

	
	// void initPoll(void);

	// void status(void);

	// int	 runPoll(void);
	// void handlePoll(void);
	// int acceptNewConnection(void); //TODO: store client info in a User object
	// void readData(int i); // TODO: data sent to server by client, so it's the main part of the server
	// void setReply(int uindex);
	// void setReply(void);
	// void sendData(int i);

	// void disconnectBadPwd(int i);
	// void disconnectBrutal(int i);
	
	// int getUserFrom(int fd) const;
	// int getUserFrom(std::string realname) const;
	// // int getPollfdFrom(int fd) const;
	// int clearUfd(int fd);



	// sockaddr_in getAdrr(void) const;
	// std::queue<Message> getWaitingList(void);
	// std::vector<Channel> getChannels(void);
};

#endif
