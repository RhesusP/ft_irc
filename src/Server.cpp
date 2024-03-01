/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 11:27:25 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

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
	PRINT_INFO("New connection detected");
	do {
		socket = accept(_servSocket, (struct sockaddr *)&usr, &size);
		if (socket == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				PRINT_ERROR("Error: failed to accept new connection");
			break;
		}
		addUser(socket, inet_ntoa(usr.sin_addr), htons(usr.sin_port));
	} while (socket != -1);
	PRINT_SUCCESS("New connection accepted");
}

void Server::readData(User &user) {
	char buf[BUFF_SIZE];
	int size;
	do
	{
		size = recv(user.getFD(), buf, BUFF_SIZE, 0);
		// std::cout << "Received " << size << " bytes from " << user.getFD() << std::endl;
		if (size == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				PRINT_ERROR("Error: failed to read data from socket " + user.getFD());
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
			formatRecv(data, user);
		}
	} while (1);
}

std::string timestr(std::time_t time)
{
	struct tm *timeinfo = gmtime(&time);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%Z", timeinfo);
	return std::string(buffer);
}

ssize_t Server::sendData(std::string message, int fd)
{
	std::time_t now = time(NULL);

	message = "@time=" + timestr(now) + + " :" + _name + " " + message;

	std::cout << "Sending message" << std::endl;
	std::cout << "\t📨: " << message;
	std::cout << "\t👤: " << fd << std::endl;

	if (message[message.size() - 1] != '\n' && message[message.size() - 2] != '\r')
		message += "\r\n";
	ssize_t size = send(fd, message.c_str(), message.size(), 0);
	if (size != message.size())
		std::cerr << "Error: failed to send message to " << fd << std::endl;
	return size;
}

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
		PRINT_INFO("Received message: '" + msg + "'");
		
		// std::cout << INFO << "Received message: '" << msg << "'" << RESET << std::endl;
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

std::string const & Server::getName(void) const
{
	return _name;
}
