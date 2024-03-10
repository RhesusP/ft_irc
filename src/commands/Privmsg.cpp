/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 01:54:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 21:04:46 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPrivmsg::CmdPrivmsg(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdPrivmsg::~CmdPrivmsg(void){}

void CmdPrivmsg::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();

	if (args.size() == 0)
	{
		reply(serv_name, ERR_NORECIPIENT(user->getNickname(), "PRIVMSG"), fd);
		return ;
	}
	else if (args.size() == 1)
	{
		reply(serv_name, ERR_NOTEXTTOSEND(user->getNickname()), fd);
		return ;
	}
	std::vector<std::string> targets = split(args[0], ",");
	std::string text = args[1];
	for (size_t i = 0; i < targets.size() ; i++)
	{
		// send message to a channel
		if (targets[i][0] == '&' || targets[i][0] == '#')
		{
			Channel *channel = _server->getChannel(targets[i]);
			if (channel == NULL)
			{
				reply(serv_name, ERR_CANNOTSENDTOCHAN(user->getNickname(), targets[i]), fd);
				continue ;
			}
			if (!user->isOnChannel(channel))
			{
				reply(serv_name, ERR_CANNOTSENDTOCHAN(user->getNickname(), targets[i]), fd);
				continue ;
			}
			channel->broadcast(user, RPL_PRIVMSG(targets[i], text));
		}
		// send message to a user
		else
		{
			User *target = _server->getUser(targets[i]);
			if (target == NULL)
			{
				reply(serv_name, ERR_NOSUCHNICK(user->getNickname(), targets[i]), fd);
				continue ;
			}
			_server->sendData(user->getNickname(), RPL_PRIVMSG(targets[i], text), target->getFD());
		}
	}
}