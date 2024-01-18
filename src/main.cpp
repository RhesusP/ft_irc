/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/18 14:39:08 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	std::string port(argv[1]);
	std::string password(argv[2]);
	Server(port, password);

	// 	int port = std::atoi(argv[1]);

	// 	std::cout << "Creating server socket..." << std::endl;
	// 	int sockfd;
	// 	int new_fd; // For new connections
	// 	// struct sigaction sa;

	// 	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	// 	if (sockfd == -1)
	// 	{
	// 		std::cerr << "socket creation failed" << std::endl;
	// 		return (1);
	// 	}
	// 	std::cout << "✅ socket creation ok (return value is " << sockfd << ")" << std::endl;

	// 	struct sockaddr_in my_addr;		// Contains my address informations
	// 	struct sockaddr_in remote_addr; // Contains remote host informations

	// 	int yes = 1;
	// 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	// 	{
	// 		std::cerr << "setsockopt error" << std::endl;
	// 		exit(1);
	// 	}

	// 	my_addr.sin_family = AF_INET;
	// 	my_addr.sin_port = htons(port);
	// 	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Use my IP
	// 	memset(&my_addr.sin_zero, '\0', 8);

	// 	// Associate that socket with a local port of the machine
	// 	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	// 	{
	// 		std::cerr << "Binding port failed" << std::endl;
	// 		return (1);
	// 	}
	// 	// TODO handle 'ALREADYINUSE' ports

	// 	// Listen for incoming connections
	// 	if (listen(sockfd, MAX_CON_QUEUE) == -1)
	// 	{
	// 		std::cerr << "Listen failed" << std::endl;
	// 		return (1);
	// 	}

	// 	// sa.sa_handler = sigchld_handler; // reap all dead processes
	// 	// sigemptyset(&sa.sa_mask);
	// 	// sa.sa_flags = SA_RESTART;
	// 	// if (sigaction(SIGCHLD, &sa, NULL) == -1)
	// 	// {
	// 	// 	perror("sigaction");
	// 	// 	exit(1);
	// 	// }

	// 	socklen_t sin_size;
	// 	while (1)
	// 	{
	// 		sin_size = sizeof(struct sockaddr_in);
	// 		new_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size);
	// 		if (new_fd == -1)
	// 		{
	// 			std::cerr << "Accept failed" << std::endl;
	// 			return (1);
	// 		}
	// 		std::cout << "accept ok" << std::endl;
	// 		char *remote_ip = inet_ntoa(remote_addr.sin_addr);
	// 		std::cout << "Server accept a connexion from remote ip " << remote_ip << std::endl;

	// 		char msg_buf[1024];

	// 		while (recv(new_fd, &msg_buf, 1024 - 2, 0) > 0)
	// 		{
	// 			msg_buf[1024 - 1] = '\0';
	// 			std::cout << "received: " << msg_buf << std::endl;
	// 		}
	// 	}
}