/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/25 14:50:50 by svanmeen         ###   ########.fr       */
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
	
	ret = poll(&_ufds[0], _nfds,-1);
	if (ret < 0)
		throw PollFailedException();
	else if (ret == 0)
		return (1);
	return (0);
}

int	Server::acceptNewConnection(void) { //TODO : implement User objs to store data and store fd
	sockaddr_in usr;
	socklen_t size = sizeof(usr); // *
	pollfd ufd;

	std::cout << "new connection" << std::endl;
	if ((ufd.fd = accept(_socket, (struct sockaddr *)&usr.sin_addr, &size)) < 0)
		throw AcceptFailedException();
	
	User user(ufd.fd, usr);
	ufd.events = POLLIN;
	_ufds.push_back(ufd);
	_users.push_back(user);
	return (1);
}

void	Server::readData(int i) { //TODO: data sent to server by client, so it's the main part of the server
	User	user = _users.at(i - 1);
	pollfd	ufd = _ufds[i];
	std::string	data;
	
	std::cout << "data to read" << std::endl;
	std::cout << "user " << inet_ntoa(user.getAddress().sin_addr) << " sent data" << std::endl;
	char	buff[BUFF_SIZE];
	int	byteread;
	
	byteread = recv(ufd.fd, buff, BUFF_SIZE - 1, 0);
	std::cout << ufd.fd << " br " << byteread << std::endl;
	if (byteread == -1)
		throw PollFailedException();
	if (byteread == 0)
		return ;
	buff[byteread] = '\0';
	data = buff;
	while (byteread == BUFF_SIZE - 1 && data.size() < 512) {
		if (byteread < 0)
			throw PollFailedException();
		byteread = recv(ufd.fd, buff, BUFF_SIZE - 1, 0);
		buff[byteread] = '\0';
		data += buff;
	}
	std::cout << "\"" << data << "\"" << std::endl;
	// if (data.compare(0, 4, "QUIT") == 0)
	// {
	// 	std::cout << "DISCONNECT" << std::endl;
	// 	_users.erase(_users.begin() + (i - 1));
	// 	_ufds.erase(_ufds.begin() + i);
	// 	_nfds -= 1;
	// 	setReply(0);
	// }
	// else
	// 	setReply(i);
}

void	Server::sendData(int i) {
	User	user = _users.at(i - 1);
	pollfd ufd = _ufds[i];
	std::string data = ":localhost 001 user :Welcome to IRC user!user@localhost\r\n"; // tease._waitingList.top().response;
	int	sizesent;

	std::cout << "data to send" << std::endl;
	std::cout << "data sent to user " << inet_ntoa(user.getAddress().sin_addr) << std::endl;
	sizesent = send(ufd.fd, data.c_str(), data.length(), 0);
	std::cout << "sent <" << sizesent << "/" << data.length() << "> bytes" << std::endl;
	_ufds.at(i).events = POLLIN;
}

void	Server::handlePoll(void) {
	int	nfds = 0;
	for (int i = 0; i < _nfds; i++) {
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
	_nfds += nfds;
}


void	Server::setReply(int uindex) {
	if (uindex == 0){
		for (int i = 1; i < _nfds; i++) {
			_ufds.at(i).events = POLLOUT;
		}
	}
	else
		_ufds.at(uindex).events = POLLOUT;
}
