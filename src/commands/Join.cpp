/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:19:56 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/02 18:00:14 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdJoin::CmdJoin(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdJoin::~CmdJoin(void){}

void CmdJoin::execute(User *user, Message *message)
{
	(void)message;
	(void)user;
}