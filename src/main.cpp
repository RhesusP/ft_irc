/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/19 15:04:14 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"
#include <stdio.h>
#include <vector>
#include <string>
#define PORT 6697
#define BUFF_SIZE 511

typedef struct s_user {
	sockaddr_in *addr;
	int			sockfd;
	int			id;
}			t_user;

int	setaddr(sockaddr_in *serv_addr, int port) { // server adress/socket init
	int sockfd;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket:");
		return(-1);
	}
	int yes = 1;
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("setsockopt");
		return(-1);
	}
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(port);
	serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(serv_addr->sin_zero), '\0', 8);

	if (bind(sockfd, (sockaddr *)serv_addr, sizeof(sockaddr)) < 0) {
		perror("bind:");
		return(-1);
	}
	if (listen(sockfd, BACKLOG) < 0){
		perror("listen:");
		return(-1);
	}
	return (sockfd);
}

pollfd	new_poll(int sockfd, short event, int *nfds) { //create new pollfd and return it
	pollfd	ufd;

	ufd.fd = sockfd;
	ufd.events = event;
	*nfds += 1;
	return (ufd);
}

void accept_new(std::vector<pollfd> *ufds) //accept new connection and add it to queue
{
	sockaddr usr;
	socklen_t size = sizeof(usr);
	pollfd	ufd;

	if ((ufd.fd = accept((*ufds)[0].fd, &usr, &size)) < 0) {
		perror("accept:");
		return ;
	}
	ufd.events = POLLIN;
	ufds->push_back(ufd);
}

void read_data(pollfd ufds) //read data from socket
{
	char	buff[BUFF_SIZE];
	int	byteread;
	
	byteread = recv(ufds.fd, buff, BUFF_SIZE - 1, 0);
	buff[byteread] = '\0';
	std::cout << "\"" << buff << "\""<< std::endl;
}

int	handle_poll(std::vector<pollfd> *ufds, int *nfds) //handle poll events
{
	for (int i = 0; i < *nfds ; i++) {
		pollfd curr = (*ufds)[i];
		if (curr.revents == POLLIN && (*ufds)[0].fd == curr.fd) {
			std::cout << "new connection" << std::endl;
			accept_new(ufds);
			*nfds += 1;
		}
		if (curr.revents == POLLIN && curr.fd != (*ufds)[0].fd) {
			std::cout << "data to read" << std::endl;
			read_data(curr);
		}
	}
	return (1);
}

int	main(int argc, char **argv) {
	// if (argc != 3) {
	// 	std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
	// 	return (1);
	// }
	// (void)argc;
	// (void)argv;

	// sockaddr_in serv_addr;
	// std::vector<pollfd> ufds;
	// int nfds = 0;
	// int servfd;

	// servfd = setaddr(&serv_addr, PORT);
	// ufds.push_back(new_poll(servfd, POLLIN, &nfds)); // add server socket to poll
	// while(1) {
	// 	if (poll(&ufds[0], nfds, 3 * 60 * 1000) <= 0){ // 3 minutes timeout
	// 		perror("poll:");
	// 		return(-1);
	// 	}
	// 	handle_poll(&ufds, &nfds);
	// }
	// std::cout << "Welcome to ft_irc 🚀" << std::endl;
	std::string port(argv[1]);
	std::string password(argv[2]);
	Server server;

	try
	{
		server.setPort(port);
		server.setPassword(password);
		std::cout << "Server initialized with with ip 127.0.0.1:" << server.getPort() << " and password " << server.getPassword() << std::endl;
		server.init_network();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
}
