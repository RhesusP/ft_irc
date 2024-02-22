/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:46:35 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 18:55:08 by cbernot          ###   ########.fr       */
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
	if (args.size() != 1)
	{
		response = ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand());
		_server->sendData(response, user->getFD());
		return;
	}
	if (user->getIsAuth())
	{
		response = ERR_ALREADYREGISTERED(user->getNickname());
		_server->sendData(response, user->getFD());
		return;
	}
	if (args[0] == _server->getPassword())
	{
		user->setAuth(true);
	}
	else
	{
		response = ERR_PASSWDMISMATCH(user->getNickname());
		_server->sendData(response, user->getFD());
	}
}