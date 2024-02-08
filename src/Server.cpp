/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/08 12:25:45 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/// @brief Look for User corresponding with `param`
/// @param fd file desctriptor
/// @return index of User associated with `param` or -1 if User not found (new User)
int	Server::getUserFrom(int fd) const {
	int	size = _users.size();
	for (int i = 0; i < size; i++) {
		if (_users.at(i).getSocket() == fd)
			return (i);
	}
	return -1;
}
int	Server::getUserFrom(std::string realname) const {
	int size = _users.size();
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

/// @brief Read data on socket then push Message on User queue. create new unregistered User if nomatch with an existing user
/// @param i index of vector<pollfd>.revents = POLLIN
void	Server::readData(int i) {
	pollfd	ufd = _ufds[i];
	int	index = Server::getUserFrom(ufd.fd);
	std::string	data;

	try {
		data = receve(ufd.fd);
	}
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		disconnectBrutal(i);
		return ;
	}
	if (index == -1) {
		User user(ufd.fd);
		//user.formatRecv(data);  TOIMPLEMENT
		if (user.getRegistered())
			disconnectBadPwd(i);
		if (getUserFrom(user.getRealName()) == -1)
			_users.push_back(user);
		// else						TOIMPLEMENT
			//update user existing
		std::cout << COYEL << data << COGRE << "from " << user.getSocket() << " is " << (user.getRegistered() ? "registered" : "unregistered") <<CORES << std::endl;
	}
	else {
		User user = _users.at(index);
		//user.formatRecv()			TOIMPLEMENT
		std::cout << COYEL << data << COGRE << "from " << user.getSocket() << " is " << (user.getRegistered() ? "registered" : "unregistered") <<CORES << std::endl;
	}
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

int		Server::clearUfd(int fd) {
	for (int i = 0; i < _nfds; i++) {
		if (_ufds.at(i).fd == fd) {
			close(_ufds.at(i).fd);
			_nfds--;
			_ufds.erase(_ufds.begin() + i);
			return (1);
		}
	}
	return (-1);
}

void	Server::status() {
	int nbconnected = 0;
	int nbuser = 0;
	int usize = _users.size();
	for (int i = 0; i < usize; i++) {
		User user = _users.at(i);
		bool ustatus = _users.at(i).getStatus();
		if (ustatus && user.getSocket() != -1) {
			if (user.getRegistered()) {
				clearUfd(user.getSocket());
				user.offline();
			}
			else {
				clearUfd(user.getSocket());
				_users.erase(_users.begin() + i);
			}
			nbconnected++;
		}
		nbuser++;
	}
}
