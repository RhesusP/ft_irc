/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:51:01 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 22:59:19 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdInvite::CmdInvite(Server *server) {
    _server = server;
    _need_auth = true;
    _need_registration = true;
}

CmdInvite::~CmdInvite(void) {}

/**
 * @brief Invite a user to a channel.
 * @details The user must be on the channel and be an operator to invite another user. The invited user will receive an
 * invitation to join the channel. If the invited user doesn't exists or is already on the channel, an error message will
 * be sent to the inviter. If the command is called without arguments, the server will send the list of invites to the
 * user.
 * @param message The INVITE message to process.
 */
void CmdInvite::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    std::string serv_name = _server->getName();
    int fd = user->getFD();

    if (args.size() > 0 && args.size() < 2) {
        reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE"), fd);
        return;
    }
    // Show invite list
    if (args.size() == 0) {
        std::list<Channel *> invites = user->getInviteList();
        for (std::list<Channel *>::iterator it = invites.begin(); it != invites.end(); it++) {
            this->reply(serv_name, RPL_INVITELIST(user->getNickname(), (*it)->getName()), fd);
        }
        this->reply(serv_name, RPL_ENDOFINVITELIST(user->getNickname()), fd);
        return;
    }
    // Invite user to channel
    std::string nick = args[0];
    std::string channel_name = args[1];
    Channel *channel = _server->getChannel(channel_name);
    User *target = _server->getUser(nick);
    if (!channel) {
        reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
        return;
    }
    if (!target) {
        reply(serv_name, ERR_NOSUCHNICK(user->getNickname(), nick), fd);
        return;
    }
    if (!user->isOnChannel(channel)) {
        reply(serv_name, ERR_NOTONCHANNEL(user->getNickname(), channel_name), fd);
        return;
    }
    if (!channel->isOperator(user)) {
        reply(serv_name, ERR_CHANOPRIVSNEEDED(user->getNickname(), channel_name), fd);
        return;
    }
    if (target->isOnChannel(channel)) {
        reply(serv_name, ERR_USERONCHANNEL(user->getNickname(), nick, channel_name), fd);
        return;
    }
    target->inviteToChannel(channel);
    reply(user->getNickname(), RPL_INVITING(user->getNickname(), nick, channel_name), fd);
    reply(user->getNickname(), "INVITE " + nick + " :" + channel_name, target->getFD());
}
