/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:16:24 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:06:56 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdUser::CmdUser(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = false;
}

CmdUser::~CmdUser(void) {}

/**
 * @brief Execute the USER command which allows a user to set its username and realname
 * @details The user is authenticated if it has a nickname, a username and a realname.
 * If the user is already registered or the command syntax is incorrect an error is returned.
 * If the user is authenticated and has a nickname, a welcome message is sent.
 * @param message The USER message to process
 */
void CmdUser::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    int fd = user->getFD();
    std::string serv_name = _server->getName();

    if (!user->getUsername().empty() || !user->getRealname().empty()) {
        this->reply(serv_name, ERR_ALREADYREGISTERED(user->getNickname()), fd);
        return;
    }
    if (!(args.size() == 4 && !args[0].empty() && args[1] == "0" && args[2] == "*" && !args[3].empty())) {
        this->reply(serv_name,
                    ERR_UNKNOWNERROR(user->getNickname(), "USER", "Bad syntax. Use /USER username 0 * :realname"), fd);
        this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), message->getCommand()), fd);
        return;
    }
    if (args[0].size() > USERLEN) {
        args[0] = args[0].substr(0, USERLEN);
    }
    user->setUsername(args[0]);
    PRINT_INFO("User " << user->getFD() << " has set its username to " << user->getUsername());
    user->setRealname(args[3]);
    PRINT_INFO("User " << user->getFD() << " has set its realname to " << user->getRealname());
    if (user->getIsAuth() && !user->getNickname().empty() && !user->getUsername().empty()) {
        welcome(message);
    }
}
