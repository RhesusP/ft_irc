/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 22:56:39 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/18 23:19:42 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPart::CmdPart(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = true;
}

CmdPart::~CmdPart(void) {}

/**
 * @brief Broadcast the list of users in a channel in response to a PART command.
 * @param server The server.
 * @param user The user who sent the PART command.
 * @param channel The channel to broadcast the list of users.
 */
void CmdPart::sendUserList(Server *server, User *user, Channel *channel) {
    std::string serv_name = server->getName();
    std::list<User *> members = channel->getRegularMembers();
    std::list<User *> chops = channel->getOperators();
    for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++) {
        channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), "@" + (*it)->getNickname()));
    }
    for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++) {
        channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), "@" + (*it)->getNickname()));
    }
    channel->broadcast(server, RPL_ENDOFNAMES(user->getNickname(), channel->getName()));
}

/**
 * @brief Remove a user from a channel.
 * @details The user must be in the channel to be removed. If the channel doesn't exist or the user is not in the channel,
 * an error message will be sent to the user.
 * @param message The PART message to process.
 */
void CmdPart::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    std::string serv_name = _server->getName();
    int fd = user->getFD();

    if (args.size() < 1) {
        this->reply(_server->getName(), ERR_NEEDMOREPARAMS(user->getNickname(), "PART"), user->getFD());
        return;
    }
    if (args.size() > 2)
        return;
    std::vector<std::string> channels = split(args[0], ",");
    std::string reason = args.size() > 1 ? args[1] : "";

    for (size_t i = 0; i < channels.size() && i < TARGMAX; i++) {
        Channel *channel = _server->getChannel(channels[i]);
        if (!channel) {
            this->reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), channels[i]), fd);
            return;
        }
        if (!channel->isInChannel(user)) {
            this->reply(serv_name, ERR_NOTONCHANNEL(user->getNickname(), channels[i]), fd);
            return;
        }
        channel->removeUser(user);
        this->reply(user->getIdentity(), "PART " + channels[i] + " :" + reason, fd);
        if (_server->getChannel(channel->getName())) {
            channel->broadcast(user, "PART " + channels[i] + " :" + reason);
            if (channel->isBotActivated()) {
                CmdBot(_server).goodbye(channel, user);
            }
            sendUserList(_server, user, channel);
        }
    }
}
