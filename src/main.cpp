/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/18 19:57:21 by cbernot          ###   ########.fr       */
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
