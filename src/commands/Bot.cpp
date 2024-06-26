/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:33:53 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 21:20:26 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdBot::CmdBot(Server *server) {
    _server = server;
    _need_auth = false;
    _need_registration = false;
    srand((unsigned) time(NULL));
}

CmdBot::~CmdBot(void) {}

/**
 * @brief Activate or deactivate the bot on a channel.
 * @details The bot will send a message to the channel to inform the users of the change. The bot can only be activated
 * by a channel operator. If the bot is already activated or deactivated, it will send a message to the channel to
 * inform the users.
 * @param message The BOT message to process.
 */
void CmdBot::execute(Message *message) {
    User *user = message->getAuthor();
    std::vector<std::string> args = message->getParameters();
    std::string serv_name = _server->getName();
    int fd = user->getFD();

    if (args.size() < 2) {
        this->reply(serv_name, ERR_NEEDMOREPARAMS(user->getNickname(), "BOT"), fd);
        return;
    }
    std::string channel_name = args[0];
    Channel *channel = _server->getChannel(channel_name);
    if (!channel) {
        this->reply(serv_name, ERR_NOSUCHCHANNEL(user->getNickname(), channel_name), fd);
        return;
    }
    if (!user->isOnChannel(channel)) {
        this->reply(serv_name, ERR_NOTONCHANNEL(user->getNickname(), channel_name), fd);
        return;
    }
    if (!channel->isOperator(user)) {
        this->reply(serv_name, ERR_CHANOPRIVSNEEDED(user->getNickname(), channel_name), fd);
        return;
    }
    if (args[1] == "on") {
        if (channel->isBotActivated()) {
            channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), "Bot is already activated on this channel."));
            return;
        }
        channel->setBotActivated(true);
        channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), user->getNickname() + " activated the bot."));
    } else if (args[1] == "off") {
        if (!channel->isBotActivated()) {
            channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), "Bot is already deactivated on this channel."));
            return;
        }
        channel->setBotActivated(false);
        channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), user->getNickname() + " deactivated the bot."));
    } else {
        this->reply(serv_name, ERR_UNKNOWNCOMMAND(user->getNickname(), args[1]), fd);
    }
}

/**
 * @brief Send a welcome message to a user when they join a channel.
 * @details The bot will send a message to the channel to welcome the user. The message is randomly chosen from a list of
 * three messages.
 * @param channel The channel the user joined.
 * @param user The user who joined the channel.
 */
void CmdBot::welcome(Channel *channel, User *user) {
    std::string messages[3] = {
            "Hey there " + user->getNickname() + ". Welcome to the party!",
            "Please, welcome Sir " + user->getNickname() + " to the channel!",
            "Welcome " + user->getNickname() + ". I hope you brought pizza!"};
    channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), messages[rand() % 3]));
}

/**
 * @brief Send a goodbye message to a user when they leave a channel.
 * @details The bot will send a message to the channel to say goodbye to the user. The message is randomly chosen from a
 * list of three messages.
 * @param channel The channel the user left.
 * @param user The user who left the channel.
 */
void CmdBot::goodbye(Channel *channel, User *user) {
    std::string messages[3] = {
            "Farewell, " + user->getNickname() + "! Remember, the door is always open for your return!",
            "Adios " + user->getNickname() + ".",
            user->getNickname() + " apparently has a life. Who knew?"};
    channel->broadcast("bot", RPL_PRIVMSG(channel->getName(), messages[rand() % 3]));
}
