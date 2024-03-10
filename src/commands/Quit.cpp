/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:25:05 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:05:43 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdQuit::CmdQuit(Server *server)
{
	_server = server;
	_need_auth = false;
	_need_registration = false;
}

CmdQuit::~CmdQuit(void) {}

void CmdQuit::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();

	std::string reason = args.size() > 0 ? args[0] : "Client quit";
	std::list<Channel *> channels = user->getChannels();
	for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		(*it)->broadcast(user, RPL_QUIT(reason));
		(*it)->removeUser(user);
	}
	_server->removeUser(fd, reason);
}
