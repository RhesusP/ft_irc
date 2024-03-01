/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:51:22 by svanmeen          #+#    #+#             */
/*   Updated: 2024/03/01 18:18:35 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(void) {
	_password = "";
	_port = 1234;
	_name = SERVER_NAME;
	_creation_time = time(NULL);
}

Server::Server(std::string port, std::string password) {
	this->setPort(port);
	this->_password = password;
	_name = SERVER_NAME;
	size = sizeof(sockaddr_in);
	_creation_time = time(NULL);
}

Server::~Server(void) {
	std::cout << "Server destructor called" << std::endl;
}

int Server::getPort(void) const {
	return _port;
}

int Server::getSocket(void) const {
	return _servSocket;
}

std::string Server::getPassword(void) const {
	return _password;
}

void Server::setPassword(std::string const &password) {
	this->_password = password;
}

void Server::setPort(std::string const &port) {
	int p;
	size_t len = port.size();
	if (len < 4 || len > 6)
		throw BadPortException();
	for (size_t i = 0; i < len; i++)
	{
		if (!isdigit(port[i]))
			throw BadPortException();
	}
	p = std::atoi(port.c_str());
	if (p < 1024 || p > 65535)
		throw BadPortException();
	this->_port = p;
}
