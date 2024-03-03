/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 16:52:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp"
#include <stdio.h>
#include <vector>
#include <string>

/*
no TimeOut for password
even if client hard quit without sendind QUIT , server respond like it has, the reason just changes and send ERROR to client
User DO NOT need to be kept in memory after logout of server
*/


int	main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "❌ Bad usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	std::string port(argv[1]);
	std::string password(argv[2]);
	try
	{
		Server server(port, password);
		server.initNetwork();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
}
