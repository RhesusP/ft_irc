/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unknown.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:04:31 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 00:08:06 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdUnknown::CmdUnknown(Server *server)
{
	_server = server;
	_need_auth = false;
	_need_registration = false;
}

CmdUnknown::~CmdUnknown(void){}

void CmdUnknown::execute(Message *message)
{
	User *user = message->getAuthor();
	this->reply(_server->getName(), ERR_UNKNOWNCOMMAND(user->getNickname(),  message->getCommand()), user->getFD());
}