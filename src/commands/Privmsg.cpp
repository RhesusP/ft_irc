/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 01:54:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:05:39 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPrivmsg::CmdPrivmsg(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = true;
}

CmdPrivmsg::~CmdPrivmsg(void) {}

/**
 * @brief Execute the PRIVMSG command which allows a user to send a message to a channel or a user
 * @details If the target is a channel, the message is broadcasted to all users on the channel
 * If the target is a user, the message is sent to the user directly.
 * @param message
 */
void CmdPrivmsg::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    std::string serv_name = _server->getName();
    int fd = user->getFD();

    if (args.empty()) {
        reply(serv_name, ERR_NORECIPIENT(user->getNickname(), "PRIVMSG"), fd);
        return;
    } else if (args.size() == 1) {
        reply(serv_name, ERR_NOTEXTTOSEND(user->getNickname()), fd);
        return;
    }
    std::vector<std::string> targets = split(args[0], ",");
    std::string text = args[1];
    for (size_t i = 0; i < targets.size(); i++) {
        // send message to a channel
        if (targets[i][0] == '&' || targets[i][0] == '#') {
            Channel *channel = _server->getChannel(targets[i]);
            if (!channel) {
                reply(serv_name, ERR_CANNOTSENDTOCHAN(user->getNickname(), targets[i]), fd);
                continue;
            }
            if (!user->isOnChannel(channel)) {
                reply(serv_name, ERR_CANNOTSENDTOCHAN(user->getNickname(), targets[i]), fd);
                continue;
            }
            channel->broadcast(user, RPL_PRIVMSG(targets[i], text));
        }
            // send message to a user
        else {
            User *target = _server->getUser(targets[i]);
            if (!target) {
                reply(serv_name, ERR_NOSUCHNICK(user->getNickname(), targets[i]), fd);
                continue;
            }
            _server->sendData(user->getNickname(), RPL_PRIVMSG(targets[i], text), target->getFD());
        }
    }
}
