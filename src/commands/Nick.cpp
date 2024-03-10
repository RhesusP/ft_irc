/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:55:13 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:03:54 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

bool CmdNick::is_nickname_valid(std::string const &nick)
{
	if (nick.size() < 1 || nick.size() > NICKLEN)
		return false;
	if (nick[0] == ':' || nick[0] == '#' || nick[0] == '&' || nick[0] == '~' || nick[0] == '+' || nick[0] == '%')
		return false;
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (nick[i] == ' ' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@' || nick[i] == '\r' || nick[i] == '\n' || nick[i] == '\t')
			return false;
	}
	return true;
}

CmdNick::CmdNick(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = false;
}

CmdNick::~CmdNick(void) {}

void CmdNick::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	int fd = user->getFD();
	std::string serv_name = _server->getName();

	if (args.size() != 1)
	{
		this->reply(serv_name, ERR_NONICKNAMEGIVEN(user->getNickname()), fd);
		return;
	}
	std::list<User *> users = _server->getUsers();

	for (std::list<User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		if ((*it)->getNickname() == args[0])
		{
			this->reply(serv_name, ERR_NICKNAMEINUSE(user->getNickname(), user->getNickname()), fd);
			return;
		}
	}
	if (!is_nickname_valid(args[0]))
	{
		this->reply(serv_name, ERR_ERRONEUSNICKNAME(user->getNickname(), user->getNickname()), fd);
		return;
	}
	user->setNickname(args[0]);
	PRINT_SUCCESS("User " << user->getFD() << " has been renamed");
	if (user->getIsAuth() && user->getNickname().size() > 0 && user->getUsername().size() > 0)
	{
		welcome(message);
	}
}
