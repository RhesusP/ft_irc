/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/29 15:29:59 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// /// @brief Look for User corresponding with `param`
// /// @param fd file desctriptor
// /// @return index of User associated with `param` or -1 if User not found (new User)
// int	Server::getUserFrom(int fd) const {
// 	int	size = _users.size();
// 	for (int i = 0; i < size; i++) {
// 		if (_users.at(i).getSocket() == fd)
// 			return (i);
// 	}
// 	return -1;
// }
// int	Server::getUserFrom(std::string realname) const {
// 	int size = _users.size();
// 	for (int i = 0; i < size; i++) {
// 		if (_users.at(i).getRealName() == realname)
// 			return (i);
// 	}
// 	return -1;
// }


void Server::addUser(int socket, char *ip, int port)
{
	User user(this, socket, ip, port);
	_users.push_back(user);
	setSocketNonBlocking(socket);
	struct pollfd ufd;
	ufd.fd = socket;
	ufd.events = POLLIN;

	_clients_fds.push_back(ufd);
}

void Server::removeUser(int socket)
{
	User user;
	size_t index;
	for (size_t i = 0 ; i < _users.size() ; i++)
	{
		if (_users.at(i).getFD() == socket)
		{
			user = _users.at(i);
			index = i;
			break;
		}
	}
	// for (size_t i = 0 ; i < _channels.size() ; i++)
	// {
	// 	if (_channels.at(i).isInChannel(user))
	// 		_channels.at(i).removeUser(user);
	// }
	_users.erase(_users.begin() + index);
	for (size_t i = 0 ; i < _clients_fds.size() ; i++)
	{
		if (_clients_fds.at(i).fd == socket)
		{
			_clients_fds.erase(_clients_fds.begin() + i);
			break;
		}
	}
	close(socket);

}

void	Server::acceptNewConnection(void) {
	int socket;
	sockaddr_in usr;
	socklen_t size = sizeof(sockaddr_in);
	std::cout << "\033[96mNew connection detected\033[39m" << std::endl;
	do {
		socket = accept(_servSocket, (struct sockaddr *)&usr, &size);
		if (socket == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				std::cerr << "\033[91mError: failed to accept new connection\033[39m" << std::endl;
			break;
		}
		addUser(socket, inet_ntoa(usr.sin_addr), htons(usr.sin_port));
	} while (socket != -1);
	std::cout << "\033[92mNew connection accepted\033[39m" << std::endl;
}

void Server::readData(User &user) {
	char buf[BUFF_SIZE];
	int size;
	do
	{
		size = recv(user.getFD(), buf, BUFF_SIZE, 0);
		std::cout << "Received " << size << " bytes from " << user.getFD() << std::endl;
		if (size == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				std::cerr << "\033[91mError: failed to read data from socket\033[39m" << std::endl;
				removeUser(user.getFD());
			}
			break;
		}
		else if (size == 0)
		{
			std::cout << "User " << user.getFD() << " disconnected" << std::endl;
			removeUser(user.getFD());
			break;
		}
		else
		{
			buf[size] = '\0';
			std::string data = buf;
			std::cout << "Data received: '" << data << "'" << std::endl;
			formatRecv(data, user);
		}
	} while (1);
}

ssize_t Server::sendData(std::string message, int fd)
{
	std::cout << "Sending message '" << message << " ' to "<< fd << std::endl;
	if (message[message.size() - 1] != '\n' && message[message.size() - 2] != '\r')
		message += "\r\n";
	ssize_t size = send(fd, message.c_str(), message.size(), 0);
	if (size != message.size())
		std::cerr << "Error: failed to send message to " << fd << std::endl;
	return size;
}

// /// @brief Read data on socket then push Message on User queue. create new unregistered User if nomatch with an existing user
// /// @param i index of vector<pollfd>.revents = POLLIN
// void	Server::readData(int i) {
// 	pollfd	ufd = _ufds[i];
// 	int	index = Server::getUserFrom(ufd.fd);
// 	std::string	data;

// 	data = receve(ufd.fd);
// 	User user = _users.at(index);
// 	formatRecv(data, user);
// 	std::cout << COGRE << "from " << user.getSocket() << " is " << (user.getRegistered() ? "registered" : "unregistered") <<CORES << std::endl;
// }

void Server::formatRecv(std::string rec, User &user)
{
	static std::string stash = "";
	std::string delimiter = "\n";
	std::string msg;
	size_t pos = 0;
	
	rec = stash + rec;
	while ((pos = rec.find(delimiter)) != std::string::npos)
	{
		if (rec.size() > 1 && rec.at(pos - 1) == '\r')
			msg = rec.substr(0, pos - 1);
		else
			msg = rec.substr(0, pos);
		std::cout << "\033[92m Message received: '" << msg << "'\033[39m" << std::endl;
		Message(this, &user, msg);
		stash = "";
		rec.erase(0, pos + delimiter.length());
	}
	stash = rec;
}

std::vector<User> Server::getUsers(void) const
{
	return _users;
}

time_t Server::getCreationTime(void) const
{
	return _creation_time;
}

std::vector<struct pollfd> Server::getClientsFds(void) const
{
	return _clients_fds;
}

// /// @brief NOT FUNCTIONAL
// /// @param i  index of _ufds
// void	Server::sendData(int i) {
// 	pollfd ufd = _ufds[i];
// 	int	index = Server::getUserFrom(ufd.fd);
// 	User	user = _users.at(index);
	
// 	std::string data = ""; // user.tease._waitingList.top().response;
// 	int	sizesent;

// 	std::cout << "data sent to user connected on socket " << ufd.fd << std::endl;
// 	sizesent = send(ufd.fd, data.c_str(), data.length(), 0);
// 	_ufds.at(i).events = POLLIN;
// }

// int		Server::clearUfd(int fd) {
// 	for (int i = 0; i < _nfds; i++) {
// 		if (_ufds.at(i).fd == fd) {
// 			close(_ufds.at(i).fd);
// 			_nfds--;
// 			_ufds.erase(_ufds.begin() + i);
// 			return (1);
// 		}
// 	}
// 	return (-1);
// }

// /// @brief Close sockets and reset/erase users disconnecting
// void	Server::status() {
// 	int usize = _users.size();
// 	for (int i = 0; i < usize; i++) {
// 		User user = _users.at(i);
// 		//if (user._reply.empty()) {
// 			bool ustatus = _users.at(i).getStatus();
// 			if (ustatus) {
// 				clearUfd(user.getSocket());
// 				_users.erase(_users.begin() + i);
// 				usize--;
// 			}
// 		// }
// 		// else {
// 			// setReply(i);
// 		// }
// 	}
// }
