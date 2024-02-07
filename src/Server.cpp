/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/07 14:02:05 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "fcntl.h"
#include "unistd.h"

std::string	receve(int fd) {
	std::string	ret;
	char		buf[BUFF_SIZE];
	int 		byteread;

	byteread = recv(fd, buf, BUFF_SIZE - 1, 0);
	if (byteread <= 0)
		throw RecvFailedException();
	buf[byteread] = '\0';
	ret = buf;
	while (byteread == BUFF_SIZE - 1 && ret.size() < 512) {
		if (byteread <= 0)
			throw RecvFailedException();
		byteread = recv(fd, buf, BUFF_SIZE - 1, 0);
		buf[byteread] = '\0';
		ret += buf;
	}
	return (ret);
}

// Constructors & Destructors

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

// Setters

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

// Methods

/// @brief init server listening, exception thrown if failed
/// @param  none
void Server::initNetwork(void) {
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw SocketFailedException();
	int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw SetsockoptFailedException();

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(this->_port);
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&_addr.sin_zero, '\0', 8);

	if (bind(_socket, (struct sockaddr *)&_addr, sizeof(struct sockaddr)) == -1)
		throw BindFailedException();
	
	if (listen(_socket, MAX_CON_QUEUE) == -1)
		throw ListenFailedException();
}

/// @brief add listening socket to pollfd vector
/// @param  none
void	Server::initPoll(void) {
	pollfd ufd;
	ufd.fd = _socket;
	ufd.events = POLLIN;
	_ufds.push_back(ufd);
	_nfds = 1;
}

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

/// @brief Look for User corresponding with `param`
/// @param fd file desctriptor
/// @return index of User associated with `param` or -1 if User not found (new User)
int	Server::getUserFrom(int fd) const {
	int	size = _users.size();
	std::cout << COYEL << size << CORES << std::endl;
	for (int i = 0; i < size; i++) {
		if (_users.at(i).getSocket() == fd)
			return (i);
	}
	return -1;
}

int	Server::getUserFrom(std::string realname) const {
	int size = _users.size();
	std::cout << COYEL << size << CORES << std::endl;
	for (int i = 0; i < size; i++) {
		if (_users.at(i).getRealName() == realname)
			return (i);
	}
	return -1;
}

/// @brief accept new nonblock connection & add pollfd to _ufds throw exception if failed
/// @param  none
/// @return 1 if no exception thrown
int	Server::acceptNewConnection(void) { //TODO : implement User objs to store data and store fd
	sockaddr_in usr;
	socklen_t size = sizeof(usr); // * <- put in private member ?
	pollfd ufd;
	int yes = 1; // <- put in private member ?

	std::cout << COGRE << "new connection" << CORES << std::endl; //LOG
	if ((ufd.fd = accept(_socket, (struct sockaddr *)&usr.sin_addr, &size)) < 0)
		throw AcceptFailedException();
	
	if (setsockopt(ufd.fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))== -1)
		throw SetsockoptFailedException();
	if (fcntl(ufd.fd, F_SETFL, O_NONBLOCK))
		throw SetsockoptFailedException();
	ufd.events = POLLIN;
	_ufds.push_back(ufd);
	return (1);
}

void Server::disconnectBadPwd(int i) {
	pollfd ufd = _ufds.at(i);
	
	close(ufd.fd);
	_ufds.erase(_ufds.begin() + i);
	_nfds--;
}

void Server::disconnectBrutal(int i) {
	pollfd ufd = _ufds.at(i);
	// User user = _users.at(getUserFrom(ufd.fd));
	
	// if (user.getRegistered())  ///LOGOUT PROCESS
	// 	user.Reset();
	// else
	// 	_users.erase(user);
	close(ufd.fd);
	_ufds.erase(_ufds.begin() + i);
	_nfds--;
}

/// @brief Read data on socket then set Message on User queue. create new unregistered User if nomatch with existing
/// @param i index of vector<pollfd>.revents = POLLIN
void	Server::readData(int i) {
	pollfd	ufd = _ufds[i];
	int	index = Server::getUserFrom(ufd.fd);
	std::string	data;
	
	std::cout << "data to read on socket " << ufd.fd << std::endl;
	try {
		data = receve(ufd.fd);
	}
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		this->disconnectBrutal(i);
	}
	std::cout << "Succes" << std::endl; //DEBUG
	if (index == -1) {
		std::cout << "index -1" << std::endl;
		User user(ufd.fd);
		//user.formatRecv(data);  TOIMPLEMENT
		if (user.getRegistered())
			disconnectBadPwd(i);
		if (getUserFrom(user.getRealName()) == -1)
			_users.push_back(user);
		// else						TOIMPLEMENT
			//update user existing
		std::cout << "New User SUcces" << std::endl;
	}
	else {
		User user = _users.at(index);
		//user.formatRecv()			TOIMPLEMENT
	}
	
	//DEBUG
	int indexx = getUserFrom(ufd.fd);
	User debug_user = _users.at(indexx);
	std::cout << COYEL << data << COGRE << "from " << debug_user.getSocket() << " is " << (debug_user.getRegistered() ? "registered" : "unregistered") <<CORES << std::endl;
}

void	Server::sendData(int i) {
	pollfd ufd = _ufds[i];
	int	index = Server::getUserFrom(ufd.fd);
	// User	user;
	// if (index >= 0)
	// 	user = _users.at(index);
	
	std::string data = ":localhost 001 user :Welcome to IRC user!user@localhost\r\n"; // user.tease._waitingList.top().response;
	int	sizesent;

	std::cout << "data sent to user connected on socket " << ufd.fd << std::endl;
	sizesent = send(ufd.fd, data.c_str(), data.length(), 0);
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
	std::cout << uindex << std::endl;
	if (uindex == 0){
		for (int i = 1; i < _nfds; i++) {
			_ufds.at(i).events = POLLOUT;
		}
	}
	else
		_ufds.at(uindex).events = POLLOUT;
}
