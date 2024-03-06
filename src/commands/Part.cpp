/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 22:56:39 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/06 12:21:02 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPart::CmdPart(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdPart::~CmdPart(void){}

void CmdPart::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();
	
	if (args.size() < 1)
	{
		this->reply(_server->getName(), ERR_NEEDMOREPARAMS(user->getNickname(), "PART"), user->getFD());
		return;
	}
	if (args.size() > 2)
		return;
	std::vector<std::string> channels = split(args[0], ",");
	std::string reason = args.size() > 1 ? args[1] : "";

	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *channel = _server->getChannel(channels[i]);
		if (!channel)
		{
			this->reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), channels[i]), fd);
			return;
		}
		if (!channel->isInChannel(user))
		{
			this->reply(serv_name, ERR_NOTONCHANNEL(user->getNickname(), channels[i]), fd);
			return;
		}
		channel->removeUser(user);
		this->reply(user->getIdentity(), "PART " + channels[i] + " :" + reason, fd);
		channel->broadcast(user, "PART " + channels[i] + " :" + reason);
	}
}