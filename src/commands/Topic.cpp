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

CmdTopic::CmdTopic(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = true;
}

CmdTopic::~CmdTopic(void) {}

/**
 * @brief Execute the TOPIC command which allows a user to set or get the topic of a channel
 * @details If the user is not on the channel or does not have the permission to set the topic, an error is returned.
 * If the user is allowed to set the topic, the topic is set and broadcasted to all users on the channel.
 * @param message The TOPIC message to process
 */
void CmdTopic::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    std::string serv_name = _server->getName();
    int fd = user->getFD();
    std::string channel_name = args[0];
    Channel *channel = _server->getChannel(channel_name);

    // only channel is given --> return topic
    if (args.size() == 1) {
        if (!channel) {
            this->reply(_server->getName(), ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
            return;
        }
        if (!user->isOnChannel(channel)) {
            this->reply(_server->getName(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()), fd);
            return;
        }
        if (!channel->getTopic().empty())
            this->reply(serv_name, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()), fd);
        else
            this->reply(serv_name, RPL_NOTOPIC(user->getNickname(), channel->getName()), fd);
    } else if (args.size() == 2) {
        // channel and topic are given --> set topic
        std::string topic = args[1];
        if (topic.size() > TOPICLEN) {
            topic = topic.substr(0, TOPICLEN);
        }
        if (!channel) {
            this->reply(_server->getName(), ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
            return;
        }
        if (!user->isOnChannel(channel)) {
            this->reply(_server->getName(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()), fd);
            return;
        }
        if (channel->isTopicRestricted() && !channel->isOperator(user)) {
            this->reply(_server->getName(), ERR_CHANOPRIVSNEEDED(user->getNickname(), channel->getName()), fd);
            return;
        }
        channel->setTopic(topic);
        channel->broadcast(_server, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()));
        std::time_t now = time(NULL);
        std::stringstream ss;
        ss << now;
        channel->broadcast(_server, RPL_TOPICWHOTIME(user->getNickname(), channel->getName(), ss.str()));
    } else {
        this->reply(_server->getName(), ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC"), fd);
    }
}
