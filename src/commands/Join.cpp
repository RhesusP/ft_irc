/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:19:56 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 19:52:44 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

bool CmdJoin::isChanNameValid (std::string const & name)
{
	if (name.size() < 1)
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == '\r' || name[i] == '\n' || name[i] == '\t')
			return false;
	}
	if (name.find("^G") != std::string::npos)
		return false;
	return true;
}

void CmdJoin::sendJoinMsg(User *user, Channel *chan)
{
	std::string serv_name = _server->getName();
	std::string user_id = user->getIdentity();
	int user_fd = user->getFD();

	this->reply(user_id, "JOIN " + chan->getName() + " * " + user_id , user_fd);
	if (chan->getTopic().size() > 0)
		this->reply(serv_name, RPL_TOPIC(user->getNickname(), chan->getName(), chan->getTopic()), user_fd);
	std::vector<User*> members = chan->getRegularMembers();
	std::vector<User*> chops = chan->getOperators();
	for (size_t i = 0; i < chops.size() ; i++)
	{
		// std::cout << *chops[i] << std::endl;
		// PRINT_WARNING("author nick: " + user->getNickname());
		// PRINT_WARNING("chan: " + chan->getName());
		// PRINT_WARNING("chop fd: " + chops[i]->getFD());
		// PRINT_WARNING("chop nick: " + chops[i]->getNickname());
		this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), "@" + chops[i]->getNickname()), user_fd);
	}
	for (size_t i = 0; i < chan->getRegularMembers().size() ; i++)
	{
		this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), members[i]->getNickname()), user_fd);
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

CmdJoin::~CmdJoin(void){}

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
	std::vector<std::string> chans = split(args[0], ",");
	std::vector<std::string> keys = args.size() == 2 ? split(args[1], ",") : std::vector<std::string>();
	std::map<std::string, std::string> chan_pwd;
	for (size_t i = 0 ; i < chans.size() ; i++)
	{
		chan_pwd[chans[i]] = keys.size() > i ? keys[i] : "";
	}

	std::map<std::string, std::string>::iterator it = chan_pwd.begin();
	while (it != chan_pwd.end())
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
			_server->addChannel(Channel(_server, name, user));
			if (it->second != "")
				_server->getChannel(it->first)->setKey(it->second);
			PRINT_INFO("Channel " << it->first << " created by " << user->getNickname());
			this->sendJoinMsg(user, _server->getChannel(it->first));
		}
		// else
		// {
		// 	// if key is wrong --> send error
		// 	if (it->second != chan->getKey())
		// 	{
		// 		this->reply(serv_name, ERR_BADCHANNELKEY(user->getNickname(), name), user_fd);
		// 		continue;
		// 	}
		// 	// if user is already in the channel --> ignore
		// 	if (!chan->addRegularMember(user))
		// 		continue;
		// 	// if channel is full --> send error
		// 	if (chan->getLimit() != -1 && (int)chan->nbMembers() + 1 > chan->getLimit())
		// 	{
		// 		this->reply(serv_name, ERR_CHANNELISFULL(user->getNickname(), name), user_fd);
		// 		continue;
		// 	}
		// 	// TODO handle channel modes
		// 	// TODO handle ban list
		// 	// TODO handle 0 argument (equivalent to part command)
		// 	else
		// 	{
		// 		this->sendJoinMsg(user, chan);
		// 	}
		// }
		it++;
	}
}