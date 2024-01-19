/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/19 16:21:37 by svanmeen         ###   ########.fr       */
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

void Server::init_network(void)
{
	int new_fd; // For new connections
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw SocketFailedException();
	std::cout << "✅ socket creation ok (return value is " << _socket << ")" << std::endl;

	sockaddr_in	remote_addr;// struct sockaddr_in remote_addr; // Contains remote host informations

	int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw SetsockoptFailedException();

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(this->_port);
	_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Use my IP
	memset(&_addr.sin_zero, '\0', 8);

	// Associate that socket with a local port of the machine
	if (bind(_socket, (struct sockaddr *)&_addr, sizeof(struct sockaddr)) == -1)
		throw BindFailedException();
	// TODO handle 'ALREADYINUSE' ports

	// Listen for incoming connections
	if (listen(_socket, MAX_CON_QUEUE) == -1)
		throw ListenFailedException();
}

void	Server::initpoll(void) {
	pollfd ufd;
	ufd.fd = _socket;
	ufd.events = POLLIN;
	_ufds.push_back(ufd);
	_nfds = 1;
}

int		Server::runPoll(void) {
	int ret;
	
	ret = poll(&_ufds[0], _nfds, 3 * 60 * 1000);
	if (ret < 0)
		throw PollFailedException();
	else if (ret == 0)
		return (1);
	return (0);
}

void	Server::acceptNewConnection(void) { //TODO : implement User objs to store data and store fd
	sockaddr_in usr; // *
	socklen_t size = sizeof(usr); // *
	
	pollfd ufd;

	if ((ufd.fd = accept(_ufds[0].fd, (struct sockaddr *)&usr, &size)) < 0) // *
		throw AcceptFailedException();
		
	ufd.events = POLLIN;
	_ufds.push_back(ufd);
	_nfds++;
}

void	Server::readData(int i) { //TODO: data sent to server by client, so it's the main part of the server
	char	buff[BUFF_SIZE];
	int	byteread;
	
	byteread = recv(_ufds[i].fd, buff, BUFF_SIZE - 1, 0);
	buff[byteread] = '\0';
	std::cout << "\"" << buff << "\""<< std::endl;
	
}

void	Server::handlePoll(void) {
	for (int i = 0; i < _nfds; i++) {
		if (_ufds[i].revents == POLLIN && _ufds[0].fd == _ufds[i].fd) {
			std::cout << "new connection" << std::endl;
			acceptNewConnection();
			_nfds += 1;
		}
		if (_ufds[i].revents == POLLIN && _ufds[i].fd != _ufds[0].fd) {
			std::cout << "data to read" << std::endl;
			readData(i);
		}
	}
}
