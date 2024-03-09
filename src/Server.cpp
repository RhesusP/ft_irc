/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/09 21:21:37 by cbernot          ###   ########.fr       */
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
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getFD() == socket)
		{
			user = *it;
			_users.erase(it);
			break;
		}
	}
	for (std::list<struct pollfd>::iterator it = _clients_fds.begin(); it != _clients_fds.end(); it++)
	{
		if (it->fd == socket)
		{
			_clients_fds.erase(it);
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

void Server::readData(User *user) {
	char buf[BUFF_SIZE];
	int size;
	do
	{
		size = recv(user->getFD(), buf, BUFF_SIZE, 0);
		if (size == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				PRINT_ERROR("Error: failed to read data from socket " + user->getFD());
				removeUser(user->getFD());
			}
			break;
		}
		else if (size == 0)
		{
			std::cout << "User " << user->getFD() << " disconnected" << std::endl;
			removeUser(user->getFD());
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

ssize_t Server::sendData(std::string sender, std::string message, int fd)
{
	std::time_t now = time(NULL);

	message = "@time=" + timestr(now) + " :" + sender + " " + message;
	if (message[message.size() - 1] != '\n' && message[message.size() - 2] != '\r')
		message += "\r\n";
	PRINT_SEND(fd, message);
	ssize_t size = send(fd, message.c_str(), message.size(), 0);
	if ((unsigned int)size != message.size())
		std::cerr << "Error: failed to send message to " << fd << std::endl;
	return size;
}

void Server::formatRecv(std::string rec, User *user)
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
		PRINT_RECEIVED(user->getFD(), msg);
		Message(this, user, msg);
		stash = "";
		rec.erase(0, pos + delimiter.length());
	}
	stash = rec;
}

std::list<User *> Server::getUsers(void)
{
	std::list<User *> res;
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		res.push_back(&(*it));
	}
	return res;
}

User* Server::getUser(std::string const &nickname)
{
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getNickname() == nickname)
			return &(*it);
	}
	return NULL;
}

time_t Server::getCreationTime(void) const
{
	return _creation_time;
}

std::list<struct pollfd> Server::getClientsFds(void)
{
	return _clients_fds;
}

std::string const & Server::getName(void) const
{
	return _name;
}

Channel *Server::addChannel(Channel channel, User *founder)
{
    if (getChannel(channel.getName()) != NULL)
    {
        PRINT_ERROR("Error: channel " + channel.getName() + " already exists");
		return NULL;
    }
    _channels.push_back(channel);
	founder->addinChannel(&(_channels.back()));
    PRINT_SUCCESS("Channel " + channel.getName() + " has been added to server");
    return &(_channels.back());
}

void Server::removeChannel(Channel channel)
{
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			break;
		}
	}
	PRINT_SUCCESS("Channel " + channel.getName() + " has been deleted from server");
}

std::list<Channel *> Server::getChannels(void)
{
	std::list<Channel *> res;
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		res.push_back(&(*it));
	}
	return res;
}

Channel *Server::getChannel(std::string const &name)
{
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->getName() == name)
			return &(*it);
	}
	return NULL;
}

std::ostream & operator<<(std::ostream & o, Server & rhs)
{
	o << "Server " << rhs.getName() << " created at " << timestr(rhs.getCreationTime()) << std::endl;
	std::list<Channel *> channels = rhs.getChannels();
	for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		o << **it << std::endl;
	}
	std::list<User *> users = rhs.getUsers();
	for (std::list<User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		o << **it << std::endl;
	}
	return o;
}