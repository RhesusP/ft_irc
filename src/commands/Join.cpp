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

CmdJoin::CmdJoin(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = true;
}

CmdJoin::~CmdJoin(void) {}

/**
 * @brief Send JOIN message to user and channel members
 * @param user The user who joined the channel
 * @param chan The channel the user joined
 */
void CmdJoin::sendJoinMsg(User *user, Channel *chan) {
    std::string serv_name = _server->getName();
    std::string user_id = user->getIdentity();
    int user_fd = user->getFD();
    std::list<User *> members = chan->getRegularMembers();
    std::list<User *> chops = chan->getOperators();

    this->reply(user_id, "JOIN " + chan->getName() + " * " + user_id, user_fd);
    if (chan->getTopic().size() > 0)
        this->reply(serv_name, RPL_TOPIC(user->getNickname(), chan->getName(), chan->getTopic()), user_fd);
    for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++) {
        this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), "@" + (*it)->getNickname()), user_fd);
    }
    for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++) {
        this->reply(serv_name, RPL_NAMREPLY(user->getNickname(), chan->getName(), (*it)->getNickname()), user_fd);
    }
    this->reply(serv_name, RPL_ENDOFNAMES(user->getNickname(), chan->getName()), user_fd);
    chan->broadcast(user, "JOIN " + chan->getName() + " * " + user_id);
    PRINT_INFO("User " << user->getNickname() << " joined channel " << chan->getName());
}

/**
 * @brief Execute the JOIN command (join a channel)
 * @details If the channel doesn't exist, it is created. If the channel is full, invite-only or password-protected, the user is notified.
 * If the user is already in the channel, the command is ignored. Otherwise, the user is added to the channel.
 * @param message The JOIN message to process.
 */
void CmdJoin::execute(Message *message) {
    User *user = message->getAuthor();
    std::string serv_name = _server->getName();
    int user_fd = user->getFD();
    std::vector<std::string> args = message->getParameters();
    std::string chan_name = "";

    if (args.size() < 1) {
        this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN"), user_fd);
        return;
    }
    // if too many parameters --> ignore
    if (args.size() > 2) {
        this->reply(serv_name, ERR_UNKNOWNERROR(user->getNickname(), "JOIN", "Too many parameters"), user_fd);
        return;
    }
    // Act as PART command for all users channels
    if (args.size() == 1 && args[0].size() == 1 && args[0][0] == '0') {
        std::list<Channel *> channels = user->getChannels();
        for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
            Message msg(_server, user, "PART " + (*it)->getName());
            CmdPart(_server).execute(&msg);
        }
        return;
    }
    std::vector<std::string> chans = split(args[0], ",");
    std::vector<std::string> keys = args.size() == 2 ? split(args[1], ",") : std::vector<std::string>();
    std::map<std::string, std::string> chan_pwd;
    for (size_t i = 0; i < chans.size() && i < TARGMAX; i++) {
        chan_pwd[chans[i]] = keys.size() > i ? keys[i] : "";
    }
    for (std::map<std::string, std::string>::iterator it = chan_pwd.begin(); it != chan_pwd.end(); it++) {
        // if channel name is invalid --> send error
        if (!isChanNameValid(it->first)) {
            this->reply(serv_name, ERR_BADCHANMASK(user->getNickname(), it->first), user_fd);
            continue;
        }
        // if channel doesn't exist --> create it and set a key if specified
        Channel *chan = _server->getChannel(it->first);
        std::string name = it->first;
        if (!chan) {
            _server->addChannel(Channel(_server, name, user), user);
            PRINT_INFO("Channel " << it->first << " created by " << user->getNickname());
            this->sendJoinMsg(user, _server->getChannel(it->first));
        } else {
            // if key is wrong --> send error
            if (!chan->getKey().empty()) {
                if (it->second.empty()) {
                    this->reply(serv_name, ERR_BADCHANNELKEY(user->getNickname(), name), user_fd);
                    continue;
                } else if (it->second != chan->getKey()) {
                    this->reply(serv_name, ERR_INVALIDKEY(user->getNickname(), name), user_fd);
                    continue;
                }
            }
            // if channel is full --> send error
            if (chan->getLimit() != -1 && (int) chan->nbMembers() + 1 > chan->getLimit()) {
                this->reply(serv_name, ERR_CHANNELISFULL(user->getNickname(), name), user_fd);
                continue;
            }
            // if channel is invite-only --> send error
            if (chan->isInviteOnly()) {
                if (user->isInvitedTo(chan)) {
                    user->removeFromInviteList(chan);
                } else {
                    this->reply(serv_name, ERR_INVITEONLYCHAN(user->getNickname(), name), user_fd);
                    continue;
                }
            }
            // if user is already in the channel --> ignore
            if (!chan->addRegularMember(user)) {
                this->reply(serv_name, ERR_USERONCHANNEL(user->getNickname(), user->getNickname(), name), user_fd);
                continue;
            } else {
                this->sendJoinMsg(user, chan);
                if (chan->isBotActivated()) {
                    CmdBot(_server).welcome(chan, user);
                }
            }
        }
    }
}
