/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:25:41 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 13:30:02 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdMotd::CmdMotd(Server *server)
{
	_server = server;
	_need_auth = false;
}

CmdMotd::~CmdMotd(void){}

void CmdMotd::execute(User *user, Message *message)
{
	std::string response;
	
	response = RPL_MOTDSTART(user->getNickname());
	_server->sendData(response, user->getFD());
	response = RPL_MOTD(user->getNickname(), "Here is the Message of the Day");
	_server->sendData(response, user->getFD());
	response = RPL_ENDOFMOTD(user->getNickname());
	_server->sendData(response, user->getFD());
}