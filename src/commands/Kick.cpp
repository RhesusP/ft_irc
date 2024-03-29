/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:27:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 14:34:55 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdKick::CmdKick(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdKick::~CmdKick(void) {}

void CmdKick::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();

	if (args.size() < 2)
	{
		reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), "KICK"), fd);
		return;
	}
	std::string channel_name = args[0];
	Channel *channel = _server->getChannel(channel_name);
	std::vector<std::string> targets = split(args[1], ",");

	if (!channel)
	{
		reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
		return;
	}
	if (!user->isOnChannel(channel))
	{
		reply(serv_name, ERR_NOTONCHANNEL(user->getNickname(), channel_name), fd);
		return;
	}
	if (!channel->isOperator(user))
	{
		reply(serv_name, ERR_CHANOPRIVSNEEDED(user->getNickname(), channel_name), fd);
		return;
	}

	for (size_t i = 0; i < targets.size(); i++)
	{
		User *target = _server->getUser(targets[i]);
		if (!target)
		{
			reply(serv_name, ERR_NOSUCHNICK(user->getNickname(), targets[i]), fd);
			continue;
		}
        if (user == target)
        {
            reply(serv_name, ERR_UNKNOWNERROR(user->getNickname(), "KICK", "you cannot kick yourself. Use /PART instead."), fd);
            return ;
        }
		if (!channel->isInChannel(target))
		{
			reply(serv_name, ERR_USERNOTINCHANNEL(user->getNickname(), targets[i], channel_name), fd);
			continue;
		}
		if (args.size() > 3)
		{
			std::string reason = args[2];
			channel->broadcast(user, "KICK " + channel_name + " " + target->getNickname() + " :" + reason);
			this->reply(user->getNickname(), "KICK " + channel_name + " " + target->getNickname() + " :" + reason, fd);
		}
		else
		{
			channel->broadcast(user, "KICK " + channel_name + " " + target->getNickname());
			this->reply(user->getNickname(), "KICK " + channel_name + " " + target->getNickname(), fd);
		}
		channel->removeUser(target);
        sendUserList(_server, user, channel, channel_name);
    }
}
