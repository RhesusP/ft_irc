/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:44:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 13:20:10 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

Command::Command(){}

Command::~Command(void){}

void Command::execute(User *user, Message message) {}

void Command::welcome(User *user)
{
	std::string response;

	response = RPL_WELCOME(user->getNickname(), user->getUsername(), user->getUsername(), user->getHostname());
	_server->sendData(response, user->getFD());
	response = RPL_YOURHOST(user->getNickname());
	_server->sendData(response, user->getFD());
	time_t creationTime = _server->getCreationTime();
	std::string creationTimeString = std::ctime(&creationTime);
	response = RPL_CREATED(user->getNickname(), creationTimeString);
	_server->sendData(response, user->getFD());
	// TODO need to send RPL_ISUPPORT
	response = RPL_LUSERCLIENT(std::to_string(_server->getUsers().size()));
	_server->sendData(response, user->getFD());
	
	response = RPL_LUSERME(std::to_string(_server->getClientsFds().size() - 1));
	_server->sendData(response, user->getFD());

	CmdMotd motd(_server);
	motd.execute(user, NULL);

}