/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:53:42 by svanmeen          #+#    #+#             */
/*   Updated: 2024/02/29 19:43:37 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

const char* setSocketNonBlockingException::what() const throw() {
	return ("setSocketNonBlocking failed");
}


/// @brief init server listening, exception thrown if failed
/// @param  none
void Server::initNetwork(void) {
	int opt_val = 1;
	_servSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (_servSocket == -1)
		throw SocketFailedException();
	if (setsockopt(_servSocket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1)
		throw SetsockoptFailedException();
	struct pollfd ufd;
	ufd.fd = _servSocket;
	ufd.events = POLLIN;
	_clients_fds.push_back(ufd);
	setSocketNonBlocking(_servSocket);

	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&addr.sin_zero, '\0', 8);

	if (bind(_servSocket, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
	{
		close(_servSocket);
		throw BindFailedException();
	}
	PRINT_SUCCESS("Server initialized on port " << this->getPort() << " and password \"" << this->getPassword() << "\" linked to socket " << this->getSocket());	
	if (listen(_servSocket, MAX_CON_QUEUE) == -1)
		throw ListenFailedException();
	PRINT_INFO("Server listening on port " << this->getPort());
	while (1)
	{
		waitingForClient();
	}
}

void Server::waitingForClient(void)
{
	pollfd ufd;
	ufd.fd = _servSocket;

	int ret = poll(&_clients_fds[0], _users.size() + 1, -1);
	
	if (ret == -1)
		throw PollFailedException();
	for (size_t i = 0 ; i < _users.size() + 1 ; i++)
	{
		if (_clients_fds[i].revents == 0)
			continue;
		if (_clients_fds[i].fd == _servSocket)
			acceptNewConnection();
		else if (i > 0)
		{
			User user = _users[i - 1];
			readData(user);
		}
	}	
}


void	Server::setSocketNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0 )
	{
		PRINT_ERROR("Error: failed to set socket non-blocking");
		if (fd != _servSocket)
			removeUser(fd);
	}
}



// /// @brief add listening socket to pollfd vector
// /// @param  none
// void	Server::initPoll(void) {
// 	pollfd ufd;
// 	ufd.fd = _servSocket;
// 	ufd.events = POLLIN;
// 	_ufds.push_back(ufd);
// 	_nfds = 1;
// }
