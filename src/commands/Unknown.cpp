/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unknown.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:04:31 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 23:06:04 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdUnknown::CmdUnknown(Server *server) {
    _server = server;
    _need_auth = false;
    _need_registration = false;
}

CmdUnknown::~CmdUnknown(void) {}

/**
 * @brief Execute the UNKNOWN command which allows a user to get an error message when the command is unknown
 * @details The user is notified that the command is unknown.
 * @param message The UNKNOWN message to process
 */
void CmdUnknown::execute(Message *message) {
    User *user = message->getAuthor();
    this->reply(_server->getName(), ERR_UNKNOWNCOMMAND(user->getNickname(), message->getCommand()), user->getFD());
}
