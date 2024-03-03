/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:25:05 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 00:08:16 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdQuit::CmdQuit(Server *server)
{
	_server = server;
	_need_auth = false;
	_need_registration = false;
}

CmdQuit::~CmdQuit(void){}

void CmdQuit::execute(Message *message)
{
	(void)message;
	// std::string response;
	// std::vector<std::string> args = message->getParameters();
	// int fd = user->getFD();

	// this->reply("ERROR: " + user->getNickname() + " quit the server.", fd);
	// // TODO send quit message to all user's channels
	
	
	// this->reply("ERROR :Closing Link: " + user->getNickname() + " (" + args[0] + ")", fd);
}