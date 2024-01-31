/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/31 18:39:46 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"
#include <stdio.h>
#include <vector>
#include <string>

int	main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	std::string port(argv[1]);
	std::string password(argv[2]);
	Server server;
	try
	{
		server.setPort(port);
		server.setPassword(password);
		server.initNetwork();
		std::cout << "✅ Server initialized on port "<< server.getPort() << " and password \"" << server.getPassword() << "\" linked to socket " << server.getSocket() << std::endl;
		server.initPoll();
		while (1 && server.runPoll() != -1) {
			server.handlePoll();
		}
		std::cout << "Server shutdown, poll error" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
}
