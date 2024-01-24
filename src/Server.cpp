/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/24 15:29:55 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(void)
{
	_password = "";
	_port = 1234;
}

Server::Server(std::string port, std::string password)
{
	this->setPort(port);
	this->_password = password;
}

Server::~Server(void)
{
	std::cout << "Server destructor called" << std::endl;
}

void Server::setPassword(std::string const &password)
{
	this->_password = password;
}

void Server::setPort(std::string const &port)
{
	int p;
	size_t len = port.size();
	if (len < 4 || len > 6)
		throw BadPortException();
	for (size_t i = 0; i < len; i++)
	{
		if (!isdigit(port[i]))
			throw BadPortException();
	}
	p = std::stoi(port);
	if (p < 1024 || p > 65535)
		throw BadPortException();
	this->_port = p;
}

std::string Server::getPassword(void) const
{
	return _password;
}

int Server::getPort(void) const
{
	return _port;
}

void Server::formatRecv(char *buf)
{
	std::string rec(buf);
	std::string msg;
	std::string delimiter = "\r\n";
	size_t pos = 0;
	while ((pos = rec.find(delimiter)) != std::string::npos)
	{
		msg = rec.substr(0, pos);
		this->_waitingList.push(Message(msg));
		// std::cout << "msg: " << msg << std::endl;
		rec.erase(0, pos + delimiter.length());
	}
}

void Server::init_network(void)
{
	int sockfd;
	int new_fd; // For new connections
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		throw SocketFailedException();
	std::cout << "✅ socket creation ok (return value is " << sockfd << ")" << std::endl;

	struct sockaddr_in my_addr;		// Contains my address informations
	struct sockaddr_in remote_addr; // Contains remote host informations

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw SetsockoptFailedException();

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(this->_port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Use my IP
	memset(&my_addr.sin_zero, '\0', 8);

	// Associate that socket with a local port of the machine
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
		throw BindFailedException();
	// TODO handle 'ALREADYINUSE' ports

	// Listen for incoming connections
	if (listen(sockfd, MAX_CON_QUEUE) == -1)
		throw ListenFailedException();

	socklen_t sin_size;
	char msg_buf[512];
	memset(msg_buf, 0, sizeof(msg_buf));
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size);
		if (new_fd == -1)
			throw AcceptFailedException();
		char *remote_ip = inet_ntoa(remote_addr.sin_addr);
		std::cout << "Server accept a connexion from remote ip " << remote_ip << std::endl;

		while (recv(new_fd, &msg_buf, 512 - 1, 0) > 0)
		{
			msg_buf[512 - 1] = '\0';
			// std::cout << "received: " << msg_buf << std::endl;
			formatRecv(msg_buf);
			// Reset buffer to avoid residual data
			memset(msg_buf, 0, sizeof(msg_buf));
		}
	}
}
