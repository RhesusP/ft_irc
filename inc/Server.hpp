/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/15 12:08:32 by svanmeen         ###   ########.fr       */
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
	sockaddr_in _addr;
	int _socket;
	int _port;
	std::string _password;
	std::vector<Channel> _channels;
	std::vector<User> _users;
	std::vector<pollfd> _ufds;
	int _nfds; // TODO ufds.size() ?
	std::queue<Message> _waitingList;
	void formatRecv(std::string rec, User &usr);

public:
	const int yes = 1;
	socklen_t size;

	Server(void);
	Server(std::string port, std::string password);
	~Server(void);
	
	void initNetwork(void);
	void initPoll(void);

	void status(void);

	int	 runPoll(void);
	void handlePoll(void);
	int acceptNewConnection(void); //TODO: store client info in a User object
	void readData(int i); // TODO: data sent to server by client, so it's the main part of the server
	void setReply(int uindex);
	void setReply(void);
	void sendData(int i);

	void disconnectBadPwd(int i);
	void disconnectBrutal(int i);
	
	int getUserFrom(int fd) const;
	int getUserFrom(std::string realname) const;
	// int getPollfdFrom(int fd) const;
	int clearUfd(int fd);

	void setPassword(std::string const &port);
	void setPort(std::string const &port);


	sockaddr_in getAdrr(void) const;
	int getPort(void) const;
	int getSocket(void) const;
	std::string getPassword(void) const;
	std::queue<Message> getWaitingList(void);
	std::vector<Channel> getChannels(void);
};

#endif
