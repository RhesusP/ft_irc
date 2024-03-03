/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:44:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 00:19:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

Command::Command(){}

Command::~Command(void){}

bool Command::getNeedAuth(void) const
{
	return _need_auth;
}

bool Command::getNeedRegistration(void) const
{
	return _need_registration;
}

void Command::execute(Message *message) {
	(void)message;
}

void Command::welcome(Message *message)
{
	User *user = message->getAuthor();
	int fd = user->getFD();
	std::string serv_name = _server->getName();
	time_t creationTime = _server->getCreationTime();
	std::string creationTimeString = std::ctime(&creationTime);
	CmdMotd motd(_server);

	this->reply(serv_name, RPL_WELCOME(user->getNickname(), serv_name, user->getIdentity()), fd);
	this->reply(serv_name, RPL_YOURHOST(user->getNickname(), serv_name), fd);
	this->reply(serv_name, RPL_CREATED(user->getNickname(), creationTimeString), fd);
	// TODO need to send RPL_ISUPPORT
	std::stringstream ss;
	ss << _server->getUsers().size();
	this->reply(serv_name, RPL_LUSERCLIENT(user->getNickname(), ss.str()), fd);
	ss.clear();
	std::stringstream ss1;
	ss1 << _server->getClientsFds().size() - 1;
	this->reply(serv_name, RPL_LUSERME(user->getNickname(), ss1.str()), fd);
	motd.execute(message);
}

int  Command::reply(std::string sender, std::string response, int fd)
{
	return (_server->sendData(sender, response, fd));
}