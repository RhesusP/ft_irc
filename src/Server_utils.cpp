/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:51:22 by svanmeen          #+#    #+#             */
/*   Updated: 2024/02/15 12:31:55 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// Constructors & Destructors

Server::Server(void) {
	_password = "";
	_port = 1234;
	
}

Server::Server(std::string port, std::string password) {
	this->setPort(port);
	this->_password = password;
	size = sizeof(sockaddr_in);
}

Server::~Server(void) {
	std::cout << "Server destructor called" << std::endl;
}

// Getters

sockaddr_in Server::getAdrr(void) const {
	return _addr;
}

int Server::getPort(void) const {
	return _port;
}

int Server::getSocket(void) const {
	return _socket;
}

std::string Server::getPassword(void) const {
	return _password;
}


// Setters

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
	p = std::stoi(port);
	if (p < 1024 || p > 65535)
		throw BadPortException();
	this->_port = p;
}


// Disconnect

void Server::disconnectBadPwd(int i) {
	pollfd ufd = _ufds.at(i);
	
	close(ufd.fd);
	_ufds.erase(_ufds.begin() + i);
	_nfds--;
}

void Server::disconnectBrutal(int i) {
	pollfd ufd = _ufds.at(i);
	int socket = ufd.fd;

	close(ufd.fd);
	_ufds.erase(_ufds.begin() + i);
	_nfds--;
	std::cerr << "Disconnect User on socket " << socket << std::endl;
}

// POLL handeler

/// @brief run poll on pollfd vector, throw exception if failed
/// @param  none
int		Server::runPoll(void) {
	int ret;
	
	ret = poll(&_ufds[0], _nfds,-1);
	if (ret < 0)
		throw PollFailedException();
	else if (ret == 0)
		return (1);
	return (0);
}

/// @brief handle revents of poll
/// @param none
void	Server::handlePoll(void) {
	int	nfds = 0;
	for (int i = 0; i < _nfds; i++) {
		try {
			if (_ufds.at(i).fd == _socket) {
				if (_ufds.at(i).revents == POLLIN)
					nfds += acceptNewConnection();
			}
			else
			{
				if (_ufds.at(i).revents == POLLOUT)
					sendData(i);
				else if (_ufds.at(i).revents == POLLIN)
					readData(i);
			}
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			disconnectBrutal(i); // addCommand(QUIT, NULL, "Connection lost");
			//if bad pwd del ufd but no user or at least check
		}
	}
	_nfds += nfds;
}

pollfd *Server::getPollfdFrom(int fd) {
	for (int i = 0; i < _nfds; i++) {
		if (_ufds.at(i).fd == fd)
			return (&_ufds.at(i));
	}
	return (NULL);
}

void	Server::setReply(std::vector<User> dest) {
	for (int i = 0; i < dest.size(); i++) {
		pollfd	*ufd = getPollfdFrom(dest.at(i).getSocket());
		if (ufd)
			ufd->events = POLLOUT;
	}
}
