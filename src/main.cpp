/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:01:30 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:10:56 by cbernot          ###   ########.fr       */
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
	try
	{
		Server server(port, password);
		server.initNetwork();
	}
	catch (std::exception &e)
	{
		std::cerr << ERROR << "Error: " << e.what() << RESET << std::endl;
		return (1);
	}
}
