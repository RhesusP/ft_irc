/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:59:13 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:05:57 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdTopic::CmdTopic(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdTopic::~CmdTopic(void) {}

void CmdTopic::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();

	std::string channel_name = args[0];
	// only channel is given --> return topic
	Channel *channel = _server->getChannel(channel_name);
	if (args.size() == 1)
	{
		if (!channel)
		{
			this->reply(_server->getName(), ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
			return;
		}
		if (!user->isOnChannel(channel))
		{
			this->reply(_server->getName(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()), fd);
			return;
		}
		if (channel->getTopic().size() > 0)
			this->reply(serv_name, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()), fd);
		else
			this->reply(serv_name, RPL_NOTOPIC(user->getNickname(), channel->getName()), fd);
	}
	// channel and topic are given --> set topic
	else if (args.size() == 2)
	{
		std::string topic = args[1];
		if (topic.size() > TOPICLEN)
		{
			topic = topic.substr(0, TOPICLEN);
		}
		if (!channel)
		{
			this->reply(_server->getName(), ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
			return;
		}
		if (!user->isOnChannel(channel))
		{
			this->reply(_server->getName(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()), fd);
			return;
		}
		if (channel->isTopicRestricted() && !channel->isOperator(user))
		{
			this->reply(_server->getName(), ERR_CHANOPRIVSNEEDED(user->getNickname(), channel->getName()), fd);
			return;
		}
		channel->setTopic(topic);
		channel->broadcast(_server, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()));
		std::time_t now = time(NULL);
		std::stringstream ss;
		ss << now;
		channel->broadcast(_server, RPL_TOPICWHOTIME(user->getNickname(), channel->getName(), ss.str()));
	}
	else
	{
		this->reply(_server->getName(), ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC"), fd);
	}
}
