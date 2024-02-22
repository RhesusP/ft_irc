/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unknown.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:04:31 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 17:59:53 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdUnknown::CmdUnknown(Server *server)
{
	_server = server;
	_need_auth = false;
}

CmdUnknown::~CmdUnknown(void){}

void CmdUnknown::execute(User *user, Message *message)
{
	// _server->sendData(std::string(ERR_UNKNOWNCOMMAND(user.getNickname(), message.getCommand())), user->getFD());
	std::string response = ERR_UNKNOWNCOMMAND(user->getNickname(),  message->getCommand());
	_server->sendData(response, user->getFD());
}