/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:16:24 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 12:19:51 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdUser::CmdUser(Server *server)
{
	_server = server;
	_need_auth = false;
}

CmdUser::~CmdUser(void){}

void CmdUser::execute(User *user, Message *message)
{
	std::string response;
	std::vector<std::string> args = message->getParameters();
	int fd = user->getFD();
	
	if (!user->getIsAuth())
	{
		this->reply(ERR_PASSWDMISMATCH(user->getNickname()), fd);
		return;
	}
	if (user->getUsername().size() > 0 || user->getRealname().size() > 0)
	{
		this->reply(ERR_ALREADYREGISTERED(user->getNickname()), fd);
		return;
	}
	if (!(args.size() == 4 && args[0].size() > 0 && args[1] == "0" && args[2] == "*" && args[3].size() > 0))
	{
		this->reply(ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
		return;
	}
	user->setUsername(args[0]);
	std::cout << "Set username to " << user->getUsername() << std::endl;
	user->setRealname(args[3]);
	std::cout << "Set realname to " << user->getRealname() << std::endl;

	if (user->getIsAuth() && user->getNickname().size() > 0 && user->getUsername().size() > 0)
	{
		welcome(user);
	}
}