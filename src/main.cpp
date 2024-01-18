/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/18 14:41:35 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"
#include <stdio.h>
#include <vector>
#define PORT 6697
#define BUFF_SIZE 511

typedef struct s_user {
	sockaddr_in *addr;
	int			sockfd;
	int			id;
}			t_user;

int	setaddr(sockaddr_in *serv_addr, int port) {
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

pollfd	new_poll(int sockfd, short event, int *nfds) {
	pollfd	ufd;

	ufd.fd = sockfd;
	ufd.events = event;
	*nfds += 1;
	return (ufd);
}

void logreceive(pollfd ufd, t_user usr)
{
	char buf[2000];
	buf[1999] = '\0';
	if (ufd.fd == usr.sockfd)
		std::cout << "ready" << std::endl;
	if (recv(ufd.fd, buf, 2000 - 1, 0) == -1)
		perror("recv");
	std::cout << "received :" << buf << std::endl;
}

void	read_poll(std::vector<t_user> &usrs, std::vector<pollfd> &ufds, int *nfds) {	
	for (int i = 0; i < *nfds; i++) {
		if (ufds[i].revents == 0)
			continue;
		if (ufds[i].revents == POLLIN && ufds[i].fd == ufds[0].fd){
			t_user	newusr;
			// memset(newusr.addr->sin_zero, 8, '\0');
			socklen_t addrlen = sizeof(newusr.addr);
			if ((newusr.sockfd = accept(ufds[0].fd, (sockaddr *)newusr.addr, &addrlen)) < 0)
				return (perror("accept"));
			std::cout << "one connection on usr " << inet_ntoa(newusr.addr->sin_addr) << std::endl;
			usrs.push_back(newusr);
			ufds.push_back(new_poll(newusr.sockfd, POLLIN, nfds));
		}
		if (ufds[i].revents == POLLIN && ufds[i].fd != ufds[0].fd)
		{
			logreceive(ufds[i], usrs[i - 1]);
		}
	}
}

void accept_new(std::vector<pollfd> *ufds)
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

void read_data(pollfd ufds)
{
	char *buf[BUFF_SIZE];
	
	buf[BUFF_SIZE - 1] = NULL;
	recv(ufds.fd, buf, BUFF_SIZE - 2, 0);
	std::cout << buf << std::endl;
}

int	handle_poll(std::vector<pollfd> *ufds, int *nfds)
{
	for (std::vector<pollfd>::iterator it = ufds->begin(); it != ufds->end(); it++) {
		pollfd curr = *it;
		if (curr.revents == POLLIN && (*ufds)[0].fd == curr.fd) {
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
	(void)argc;
	(void)argv;
	//server
	// sockaddr_in serv_addr;
	// int	sockfd;
	// int nfds;
	// std::vector<pollfd> ufds;
	// std::vector<t_user> usrs;

	// nfds = 0;
	// sockfd = setaddr(&serv_addr, argv[1]);
	// if (sockfd < 0)
	// 	return (1);
	// ufds.push_back(new_poll(sockfd, POLLIN, &nfds));
	// while (1) {
	// 	if (poll(&ufds[0], nfds, 3 * 60 * 1000) < 0)
	// 		return (-1);
	// 	read_poll(usrs, ufds, &nfds);
	// }
	sockaddr_in serv_addr;
	std::vector<pollfd> ufds;
	int nfds = 0;
	int servfd;

	servfd = setaddr(&serv_addr, PORT);
	ufds.push_back(new_poll(servfd, POLLIN, &nfds));
	while(1) {
		if (poll(&ufds[0], nfds, 3 * 60 * 1000) <= 0){
			perror("poll:");
			return(-1);
		}
		std::cout << "poll found" << std::endl;
		handle_poll(&ufds, &nfds);
		
	}
	std::cout << "Welcome to ft_irc 🚀" << std::endl;
}
