/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:25:05 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/05 12:53:03 by svanmeen         ###   ########.fr       */
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
	std::vector<std::string> args = message->getParameters();
	User *usr = message->getAuthor();

	usr->reason = args[0];
	for (size_t i = 0; i < usr->getChannels().size(); i++)
	{
		usr->getChannels().at(i)->removeUser(usr);
	}
	this->reply(usr->getIdentity(), "ERROR :Disconnected\r\n", usr->getFD());
	_server->removeUser(usr->getFD());
	// std::string response;
	// std::vector<std::string> args = message->getParameters();
	// int fd = user->getFD();

	// this->reply("ERROR: " + user->getNickname() + " quit the server.", fd);
	// // TODO send quit message to all user's channels
	
	
	// this->reply("ERROR :Closing Link: " + user->getNickname() + " (" + args[0] + ")", fd);
}
