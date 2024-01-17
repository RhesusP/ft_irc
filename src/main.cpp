/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/17 18:18:51 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"
#include <stdio.h>
#include <vector>

typedef struct s_user {
	sockaddr_in addr;
	int			sockfd;
	int			id;
}			t_user;

int	setaddr(sockaddr_in *serv_addr, char *port) {
	int sockfd;

	(void)port;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(6697);
	serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(serv_addr->sin_zero), '\0', 8);

	if (bind(sockfd, (sockaddr *)serv_addr, sizeof(sockaddr)) < 0)
		return (-1);
	if (listen(sockfd, BACKLOG) < 0)
		return (-1);
	return (sockfd);
}

int	main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	//server
	sockaddr_in serv_addr;
	int nfds;
	int	i;
	std::vector<pollfd> ufds;
	std::vector<t_user> usrs;

	int	sockfd;
	sockfd = setaddr(&serv_addr, argv[1]);
	if (sockfd < 0)
		return (1);
	i = 0;
	nfds = 0;
	while (1)
	{
		t_user newusr;
		pollfd newpoll;
		if (newusr.sockfd = accept(sockfd, (sockaddr *)&(newusr.addr), &(sizeof(sockaddr_in))) != -1) {
			usrs.push_back(newusr);
			newpoll.fd = newusr.sockfd;
			newpoll.events = POLLIN;
			ufds.push_back(newpoll);
			nfds++;
			i++;
		}
	}

	(void)argv;
	std::cout << "Welcome to ft_irc 🚀" << std::endl;
}
