/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:24:47 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:08:59 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdWho::CmdWho(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = false;
}

CmdWho::~CmdWho(void) {}

/**
 * @brief Execute the WHO command which allows a user to get information about a user or a channel
 * @details The user can get information about a user or a channel.
 * If the target is a channel, the user gets information about all users on the channel.
 * If the target is a user, the user gets information about the user.
 * @param message The WHO message to process
 */
void CmdWho::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    int fd = user->getFD();
    std::string serv_name = _server->getName();

    if (!args.empty()) {
        std::string mask = args[0];
        // search for a channel
        if (mask[0] == '#' || mask[0] == '&') {
            Channel *channel = _server->getChannel(mask);
            if (!channel) {
                this->reply(serv_name, RPL_ENDOFWHO(user->getNickname(), mask), fd);
                return;
            }
            std::list<User *> chops = channel->getOperators();
            for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++) {
                this->reply(serv_name,
                            RPL_WHOREPLY(user->getNickname(), mask, (*it)->getUsername(), (*it)->getHostname(),
                                         (*it)->getNickname(), "H@", (*it)->getRealname()), fd);
            }
            std::list<User *> users = channel->getRegularMembers();
            for (std::list<User *>::iterator it = users.begin(); it != users.end(); it++) {
                this->reply(serv_name,
                            RPL_WHOREPLY(user->getNickname(), mask, (*it)->getUsername(), (*it)->getHostname(),
                                         (*it)->getNickname(), "H", (*it)->getRealname()), fd);
            }
            this->reply(serv_name, RPL_ENDOFWHO(user->getNickname(), mask), fd);
        } else {
            // search for a user
            User *target = _server->getUser(mask);
            if (!target) {
                this->reply(serv_name, RPL_ENDOFWHO(user->getNickname(), mask), fd);
                return;
            } else {
                std::string flags = "H";
                if (target->isRegistered())
                    flags += "r";
                std::list<Channel *> channels = target->getChannels();
                for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
                    if ((*it)->isOperator(target)) {
                        flags += "@";
                        break;
                    }
                }
                this->reply(serv_name,
                            RPL_WHOREPLY(user->getNickname(), "", target->getUsername(), target->getHostname(),
                                         target->getNickname(), flags, target->getRealname()), fd);
                this->reply(serv_name, RPL_ENDOFWHO(user->getNickname(), mask), fd);
            }
        }
    }
}
