/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:44:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:44:31 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

Command::Command() {}

Command::~Command(void) {}

/**
 * @brief Returns the _need_auth attribute of the command
 * @return bool (true if the command needs the user to be authenticated)
 */
bool Command::getNeedAuth(void) const {
    return _need_auth;
}

/**
 * @brief Returns the _need_registration attribute of the command
 * @return bool (true if the command needs the user to be registered)
 */
bool Command::getNeedRegistration(void) const {
    return _need_registration;
}

void Command::execute(Message *message) {
    (void) message;
}

/**
 * @brief Sends a message to the user
 * @param sender The nickname of the sender
 * @param response The message to send
 * @param fd The file descriptor of the user
 * @return int (0 if the message was sent successfully, -1 otherwise)
 */
int Command::reply(std::string sender, std::string response, int fd) {
    return (_server->sendData(sender, response, fd));
}

/**
 * @brief Sends a welcome message to the user
 * @details Sends the welcome message to the new user, according to the RFC 1459
 * @param message The message received by the server (NICK or USER command)
 */
void Command::welcome(Message *message) {
    User *user = message->getAuthor();
    int fd = user->getFD();
    std::string serv_name = _server->getName();
    time_t creationTime = _server->getCreationTime();
    std::string creationTimeString = std::ctime(&creationTime);
    std::stringstream support, ss, ss1;
    CmdMotd motd(_server);

    this->reply(serv_name, RPL_WELCOME(user->getNickname(), serv_name, user->getIdentity()), fd);
    this->reply(serv_name, RPL_YOURHOST(user->getNickname(), serv_name), fd);
    this->reply(serv_name, RPL_CREATED(user->getNickname(), creationTimeString), fd);
    support << "CASEMAPPING=ascii";
    support << ",CHANNELLEN=" << CHANNELLEN;
    support << ",CHANTYPES=#";
    support << ",NICKLEN=" << NICKLEN;
    support << "SILENCE";
    support << ",TARGMAX=" << TARGMAX;
    support << ",TOPICLEN=" << TOPICLEN;
    support << ",USERLEN=" << USERLEN;
    this->reply(serv_name, RPL_ISUPPORT(user->getNickname(), support.str()), fd);
    ss << _server->getUsers().size();
    this->reply(serv_name, RPL_LUSERCLIENT(user->getNickname(), ss.str()), fd);
    ss1 << _server->getClientsFds().size() - 1;
    this->reply(serv_name, RPL_LUSERME(user->getNickname(), ss1.str()), fd);
    motd.execute(message);
}
