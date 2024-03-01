/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 19:35:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/29 20:19:04 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPing::CmdPing(Server *server)
{
	_server = server;
	_need_auth = false;
}

CmdPing::~CmdPing(void){}

void CmdPing::execute(User *user, Message *message)
{
	std::string response;
	std::vector<std::string> args = message->getParameters();
	int fd = user->getFD();

	if (args.size() != 1)
	{
		this->reply(ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
		return;
	}
	this->reply("PONG " + _server->getName() + " :" + args[0], fd);
}