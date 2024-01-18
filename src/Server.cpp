/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/18 19:31:28 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int getPort(std::string p)
{
	int port;
	size_t len = p.size();
	if (len < 4 || len > 6)
		return -1;
	for (size_t i = 0; i < len; i++)
	{
		if (!isdigit(p[i]))
			return -1;
	}
	port = std::stoi(p);
	if (port < 1024 || port > 65535)
		return -1;
	return port;
}

Server::Server(std::string port, std::string password)
{
	int p = ::getPort(port);
	if (p < 0)
		throw BadPortException();
	this->_port = p;
	this->_password = password;
	std::cout << "Server initialized with with ip 127.0.0.1:" << this->getPort() << " and password " << this->getPassword() << std::endl;
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