/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/18 23:20:37 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"

Server server;

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "Caught signal " << sig << std::endl;
		server.shutdown();
		exit(0);
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << ERROR << "Error: sigaction failed" << RESET << std::endl;
		return (1);
	}
	std::string port(argv[1]);
	std::string password(argv[2]);
	try
	{
		server.setPort(port);
		server.setPassword(password);
		server.initNetwork();
	}
	catch (std::exception &e)
	{
		std::cerr << ERROR << "Error: " << e.what() << RESET << std::endl;
		return (1);
	}
}