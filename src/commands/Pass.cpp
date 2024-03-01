/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:46:35 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/29 20:18:03 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPass::CmdPass(Server *server)
{
	_server = server;
	_need_auth = false;
}

CmdPass::~CmdPass(void){}

void CmdPass::execute(User *user, Message *message)
{
	std::string response;
	std::vector<std::string> args = message->getParameters();
	int fd = user->getFD();

	if (args.size() != 1)
	{
		this->reply(ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
		return;
	}
	if (user->getIsAuth())
	{
		this->reply(ERR_ALREADYREGISTERED(user->getNickname()), fd);
		return;
	}
	if (args[0] == _server->getPassword())
	{
		user->setAuth(true);
		return ;
	}
	else
	{
		this->reply(ERR_PASSWDMISMATCH(user->getNickname()), fd);
	}
}