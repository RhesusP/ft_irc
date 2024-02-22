/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unknown.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:04:31 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/29 20:19:59 by cbernot          ###   ########.fr       */
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
	this->reply(ERR_UNKNOWNCOMMAND(user->getNickname(),  message->getCommand()), user->getFD());
}