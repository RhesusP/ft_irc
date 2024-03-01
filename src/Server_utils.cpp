/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:51:22 by svanmeen          #+#    #+#             */
/*   Updated: 2024/02/29 19:44:03 by cbernot          ###   ########.fr       */
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
	p = std::stoi(port);
	if (p < 1024 || p > 65535)
		throw BadPortException();
	this->_port = p;
}


// // Disconnect
// void Server::disconnectBadPwd(int i) {
// 	pollfd ufd = _ufds.at(i);
	
// 	close(ufd.fd);
// 	_ufds.erase(_ufds.begin() + i);
// 	_nfds--;
// }

// void Server::disconnectBrutal(int i) {
// 	pollfd ufd = _ufds.at(i);
// 	int socket = ufd.fd;

// 	close(ufd.fd);
// 	_ufds.erase(_ufds.begin() + i);
// 	_nfds--;
// 	std::cerr << "Disconnect User on socket " << socket << std::endl;
// }

// // POLL handeler

// /// @brief run poll on pollfd vector, throw exception if failed
// /// @param  none
// int		Server::runPoll(void) {
// 	int ret;
	
// 	ret = poll(&_ufds[0], _nfds,-1);
// 	if (ret < 0)
// 		throw PollFailedException();
// 	else if (ret == 0)
// 		return (1);
// 	return (0);
// }

// /// @brief handle revents of poll
// /// @param none
// void	Server::handlePoll(void) {
// 	int	nfds = 0;
// 	for (int i = 0; i < _nfds; i++) {
// 		try {
// 			if (_ufds.at(i).fd == _servSocket) {
// 				if (_ufds.at(i).revents == POLLIN)
// 					nfds += acceptNewConnection();
// 			}
// 			else
// 			{
// 				if (_ufds.at(i).revents == POLLOUT)
// 					sendData(i);
// 				else if (_ufds.at(i).revents == POLLIN)
// 					readData(i);
// 			}
// 		}
// 		catch (std::exception &e) {
// 			std::cerr << e.what() << std::endl;
// 			disconnectBrutal(i); // addCommand(QUIT, NULL, "Connection lost");
// 			//if bad pwd del ufd but no user or at least check
// 		}
// 	}
// 	_nfds += nfds;
// }


// void	Server::setReply(int uindex) {
// 	std::cout << uindex << std::endl;
// 	if (uindex == 0){
// 		for (int i = 1; i < _nfds; i++) {
// 			_ufds.at(i).events = POLLOUT;
// 		}
// 	}
// 	else
// 		_ufds.at(uindex).events = POLLOUT;
// }
