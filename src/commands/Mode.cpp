/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 09:07:07 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:02:34 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdMode::CmdMode(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = false;
}

CmdMode::~CmdMode(void) {}

void CmdMode::handleModeString(std::vector<std::string> args, Channel *chan, User *user)
{
	std::string modeString = args[1];
	std::vector<std::string> mode_args;

	for (size_t i = 2; i < args.size(); i++)
	{
		std::vector<std::string> split_args = split(args[i], " ");
		for (size_t j = 0; j < split_args.size(); j++)
		{
			mode_args.push_back(split_args[j]);
		}
	}
	if (modeString[0] != '+' && modeString[0] != '-')
	{
		this->reply(_server->getName(), ERR_UMODEUNKNOWNFLAG(user->getNickname()), 0);
		return;
	}
	bool add;
	size_t j = 0;
	for (size_t i = 0; i < modeString.size(); i++)
	{
		if (modeString[i] == '+' || modeString[i] == '-')
		{
			add = modeString[i] == '+' ? true : false;
			continue;
		}
		add ? chan->addMode(modeString[i], j < mode_args.size() ? mode_args[j] : "", user) : chan->removeMode(modeString[i], j < mode_args.size() ? mode_args[j] : "", user);
		j++;
	}
	(void)chan;
}

void CmdMode::execute(Message *message)
{
	User *user = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string serv_name = _server->getName();
	int fd = user->getFD();

	if (args.size() < 1)
	{
		this->reply(_server->getName(), ERR_NEEDMOREPARAMS(user->getNickname(), "MODE"), user->getFD());
		return;
	}
	// User mode
	if (args[0][0] != '#' && args[0][0] != '&')
	{
		User *target = _server->getUser(args[0]);
		// If user does not exist --> ERR_NOSUCHNICK
		if (!target)
		{
			this->reply(serv_name, ERR_NOSUCHNICK(user->getNickname(), args[0]), fd);
			return;
		}
		// If user is not the same as the one sending the command --> ERR_USERSDONTMATCH
		if (target->getNickname() != user->getNickname())
		{
			this->reply(serv_name, ERR_USERSDONTMATCH(user->getNickname()), fd);
			return;
		}
		// If no mode is given --> RPL_UMODEIS
		if (args.size() == 1)
		{
			if (user->isRegistered())
			{
				this->reply(serv_name, RPL_UMODEIS(user->getNickname(), "+r"), fd);
				return;
			}
			else
			{
				this->reply(serv_name, RPL_UMODEIS(user->getNickname(), "-r"), fd);
				return;
			}
		}
		// If mode is given --> ERR_UMODEUNKNOWNFLAG
		else
		{
			this->reply(serv_name, ERR_UMODEUNKNOWNFLAG(user->getNickname()), fd);
			return;
		}
	}
	// Channel mode
	else
	{
		Channel *target = _server->getChannel(args[0]);
		// If channel does not exist --> ERR_NOSUCHCHANNEL
		if (!target)
		{
			this->reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), args[0]), fd);
			return;
		}
		if (args.size() == 1)
		{
			// If no mode is given --> RPL_CHANNELMODEIS and RPL_CREATIONTIME
			this->reply(serv_name, RPL_CHANNELMODEIS(user->getNickname(), target->getName(), target->getModes()), fd);
			this->reply(serv_name, RPL_CREATIONTIME(user->getNickname(), target->getName(), timestr(target->getCreationTime())), fd);
			return;
		}
		// If modestring is given
		if (args.size() >= 2)
		{
			if (!target->isOperator(user))
			{
				this->reply(serv_name, ERR_CHANOPRIVSNEEDED(user->getNickname(), target->getName()), fd);
				return;
			}
			// If modestring is valid --> change modes
			handleModeString(args, target, user);
		}
	}
}
