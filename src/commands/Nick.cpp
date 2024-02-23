/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:55:13 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 12:11:34 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

bool is_nickname_valid(std::string const & nick)
{
	if (nick.size() < 1)
		return false;
	if (nick[0] == ':' || nick[0] == '#' || nick[0] == '&')
		return false;
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (nick[i] == ' ' || nick[i] == '\r' || nick[i] == '\n' || nick[i] == '\t')
			return false;
	}
	return true;
}

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

	if (!user->getIsAuth())
	{
		response = ERR_PASSWDMISMATCH(user->getNickname());
		_server->sendData(response, user->getFD());
		return;
	}
	if (args.size() != 1)
	{
		response = ERR_NONICKNAMEGIVEN(user->getNickname());
		_server->sendData(response, user->getFD());
		return;
	}
	std::vector<User> users = _server->getUsers();

	for (size_t i = 0 ; i < users.size() ; i++)
	{
		if (users[i].getNickname() == args[0])
		{
			response = ERR_NICKNAMEINUSE(user->getNickname(), user->getNickname());
			_server->sendData(response, user->getFD());
			return;
		}
	}
	if (!is_nickname_valid(args[0]))
	{
		response = ERR_ERRONEUSNICKNAME(user->getNickname(), user->getNickname());
		_server->sendData(response, user->getFD());
		return;
	}
	user->setNickname(args[0]);
	std::cout << "User " << user->getNickname() << " has been renamed" << std::endl;

	if (user->getIsAuth() && user->getNickname().size() > 0 && user->getUsername().size() > 0)
	{
		welcome(user);
	}
}
