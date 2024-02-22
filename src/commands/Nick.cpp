/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:55:13 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 19:22:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdNick::CmdNick(Server *server)
{
	_server = server;
	_need_auth = true;
}

CmdNick::~CmdNick(void){}

void CmdNick::execute(User *user, Message *message)
{
	std::string response;
	std::vector<std::string> args = message->getParameters();

	if (user->getIsAuth())
	{
		response = ERR_PASSWDMISMATCH(user->getNickname());
		_server->sendData(response, user->getFD());
		return;
	}

	

	if (args.size() != 1)
	{
		response = ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand());
		_server->sendData(response, user->getFD());
		return;
	}

	// TODO 

	// if (!user->getIsAuth())
	// {
	// 	response = ERR_PASSWDMISMATCH(user->getNickname());
	// 	_server->sendData(response, user->getFD());
	// 	return;
	// }
	// if ()
}