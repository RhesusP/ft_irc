/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:53:42 by svanmeen          #+#    #+#             */
/*   Updated: 2024/02/15 10:36:17 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/// @brief init server listening, exception thrown if failed
/// @param  none
void Server::initNetwork(void) {
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw SocketFailedException();
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
