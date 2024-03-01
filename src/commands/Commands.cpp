/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:44:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/01 12:16:11 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

Command::Command(){}

Command::~Command(void){}

bool Command::getNeedAuth(void) const
{
	return _need_auth;
}

void Command::execute(User *user, Message *message) {}

void Command::welcome(User *user)
{
	std::string response;
	int fd = user->getFD();
	time_t creationTime = _server->getCreationTime();
	std::string creationTimeString = std::ctime(&creationTime);
	CmdMotd motd(_server);

	this->reply(RPL_WELCOME(user->getNickname(), _server->getName(), user->getIdentity()), fd);
	this->reply(RPL_YOURHOST(user->getNickname(), _server->getName()), fd);
	this->reply(RPL_CREATED(user->getNickname(), creationTimeString), fd);
	// TODO need to send RPL_ISUPPORT
	this->reply(RPL_LUSERCLIENT(user->getNickname(), std::to_string(_server->getUsers().size())), fd);
	this->reply(RPL_LUSERME(user->getNickname(), std::to_string(_server->getClientsFds().size() - 1)), fd);
	motd.execute(user, NULL);
}

int  Command::reply(std::string response, int fd)
{
	return (_server->sendData(response, fd));
}