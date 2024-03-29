/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:46:35 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:05:14 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdPass::CmdPass(Server *server) {
    _server = server;
    _need_auth = false;
    _need_registration = false;
}

CmdPass::~CmdPass(void) {}

/**
 * @brief Execute the PASS command which allows a user to authenticate
 *
 * @param message The PASS message to process
 */
void CmdPass::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    int fd = user->getFD();
    std::string serv_name = _server->getName();

    if (args.empty()) {
        this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
        return;
    }
    if (args.size() > 1) {
        this->reply(serv_name, ERR_UNKNOWNERROR(user->getNickname(), "PASS", "Too many arguments"), fd);
        return;
    }
    if (user->getIsAuth()) {
        this->reply(serv_name, ERR_ALREADYREGISTERED(user->getNickname()), fd);
        return;
    }
    if (args[0] == _server->getPassword()) {
        user->setIsAuth(true);
        PRINT_SUCCESS("User " << user->getFD() << " is now authenticated");
        return;
    } else {
        this->reply(serv_name, ERR_PASSWDMISMATCH(user->getNickname()), fd);
    }
}
