/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 19:35:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:05:29 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPing::CmdPing(Server *server) {
    _server = server;
    _need_auth = false;
    _need_registration = false;
}

CmdPing::~CmdPing(void) {}

/**
 * @brief Execute the PING command which allows a user to check if the server is alive
 * @details The server will reply with a PONG message containing the server name and the ID of the PING message
 * @param message
 */
void CmdPing::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    int fd = user->getFD();
    std::string serv_name = _server->getName();

    if (args.size() != 1) {
        this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
        return;
    }
    this->reply(serv_name, "PONG " + _server->getName() + " :" + args[0], fd);
}
