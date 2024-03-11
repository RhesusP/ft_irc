/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:19:56 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/11 17:41:59 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

bool CmdJoin::isChanNameValid(std::string const &name)
{
	if (name.size() < 1 || name.size() > CHANNELLEN)
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == '\r' || name[i] == '\n' || name[i] == '\t')
			return false;
		if (i > 0 && name[i] == 'G' && name[i - 1] == '^')
			return false;
	}
	return true;
}

void CmdJoin::sendJoinMsg(User *user, Channel *chan)
{
	std::string serv_name = _server->getName();
	std::string user_id = user->getIdentity();
	int user_fd = user->getFD();

	this->reply(user_id, "JOIN " + chan->getName() + " * " + user_id, user_fd);
	if (chan->getTopic().size() > 0)
		this->reply(serv_name, RPL_TOPIC(user->getNickname(), chan->getName(), chan->getTopic()), user_fd);
	std::list<User *> members = chan->getRegularMembers();
	std::list<User *> chops = chan->getOperators();
	for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++)
	{
		this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), "@" + (*it)->getNickname()), user_fd);
	}
	for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++)
	{
		this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), (*it)->getNickname()), user_fd);
	}
	this->reply(serv_name, RPL_ENDOFNAMES(user->getNickname(), chan->getName()), user_fd);

	// send message to all users in the channel
	chan->broadcast(user, "JOIN " + chan->getName() + " * " + user_id);
	PRINT_INFO("User " << user->getNickname() << " joined channel " << chan->getName());
}

CmdJoin::CmdJoin(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdJoin::~CmdJoin(void) {}

// no max channels for a user
void CmdJoin::execute(Message *message)
{
	User *user = message->getAuthor();
	std::string serv_name = _server->getName();
	int user_fd = user->getFD();
	std::vector<std::string> args = message->getParameters();

	if (args.size() < 1)
	{
		this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN"), user_fd);
		return;
	}
	// if too many parameters --> ignore
	if (args.size() > 2)
		return;
	// Act as PART command for all users channels
	if (args.size() == 1 && args[0].size() == 1 && args[0][0] == '0')
	{
		std::list<Channel *> channels = user->getChannels();
		for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			Channel *chan = *it;
			chan->removeUser(user);
			this->reply(user->getNickname(), "PART " + chan->getName(), user_fd);
			chan->broadcast(user, "PART " + chan->getName());
			PRINT_INFO("User " << user->getNickname() << " left channel " << chan->getName());
		}
		return;
	}
	std::vector<std::string> chans = split(args[0], ",");
	std::vector<std::string> keys = args.size() == 2 ? split(args[1], ",") : std::vector<std::string>();
	std::map<std::string, std::string> chan_pwd;
	for (size_t i = 0; i < chans.size() && i < TARGMAX; i++)
	{
		chan_pwd[chans[i]] = keys.size() > i ? keys[i] : "";
	}
	for (std::map<std::string, std::string>::iterator it = chan_pwd.begin(); it != chan_pwd.end(); it++)
	{
		// if channel name is invalid --> send error
		if (!isChanNameValid(it->first))
		{
			this->reply(serv_name, ERR_BADCHANMASK(user->getNickname(), it->first), user_fd);
			continue;
		}
		// if channel doesn't exist --> create it and set a key if specified
		Channel *chan = _server->getChannel(it->first);
		std::string name = it->first;
		if (chan == NULL)
		{
			_server->addChannel(Channel(_server, name, user), user);
			PRINT_INFO("Channel " << it->first << " created by " << user->getNickname());
			this->sendJoinMsg(user, _server->getChannel(it->first));
		}
		else
		{
			// if key is wrong --> send error
			if (chan->getKey().size() > 0)
			{
				if (it->second.size() == 0)
				{
					this->reply(serv_name, ERR_BADCHANNELKEY(user->getNickname(), name), user_fd);
					continue;
				}
				else if (it->second != chan->getKey())
				{
					this->reply(serv_name, ERR_INVALIDKEY(user->getNickname(), name), user_fd);
					continue;
				}
			}
			// if channel is full --> send error
			if (chan->getLimit() != -1 && (int)chan->nbMembers() + 1 > chan->getLimit())
			{
				this->reply(serv_name, ERR_CHANNELISFULL(user->getNickname(), name), user_fd);
				continue;
			}
			// if channel is invite-only --> send error
			if (chan->isInviteOnly())
			{
				if (user->isInvitedTo(chan))
				{
					user->removeFromInviteList(chan);
				}
				else
				{
					this->reply(serv_name, ERR_INVITEONLYCHAN(user->getNickname(), name), user_fd);
					continue;
				}
			}
			// if user is already in the channel --> ignore
			if (!chan->addRegularMember(user))
			{
				this->reply(serv_name, ERR_USERONCHANNEL(user->getNickname(), user->getNickname(), name), user_fd);
				continue;
			}
			else
			{
				this->sendJoinMsg(user, chan);
				if (chan->isBotActivated())
				{
					CmdBot(_server).welcome(chan, user);
				}
			}
		}
	}
}
