/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:40:51 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 12:44:17 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

/**
 * @brief Split a string using a string as delimiter
 * @param str String to split
 * @param delimiter Delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(std::string str, std::string const &delimiter) {
    std::vector<std::string> res;
    size_t pos = 0;
    size_t del_len = delimiter.size();
    while ((pos = str.find(delimiter)) != std::string::npos) {
        res.push_back(str.substr(0, pos));
        str.erase(0, pos + del_len);
    }
    if (!str.empty()) {
        res.push_back(str);
    }
    return res;
}

/**
 * @brief Trim a string from a given charset
 * @param str
 * @param charset
 * @return std::string Trimmed string
 */
std::string trim(const std::string &str, std::string const &charset) {
    size_t first = str.find_first_not_of(charset);
    if (first == std::string::npos) {
        return str;
    }
    size_t last = str.find_last_not_of(charset);
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Convert a time_t to a string in the format "YYYY-MM-DDTHH:MM:SS.Z"
 * @param time
 * @return std::string
 */
std::string timestr(std::time_t time) {
    struct tm *timeinfo = gmtime(&time);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%Z", timeinfo);
    return std::string(buffer);
}

/**
 * @brief Convert a string to uppercase
 * @param str
 * @return std::string Uppercase string
 */
std::string const &toupper(std::string &str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = std::toupper(str[i]);
    }
    return str;
}

/**
 * @brief Sends the list of users on a channel
 * @param server The server
 * @param user The user who wants the list
 * @param channel The channel to list
 * @param chan_name The name of the channel
 */
void sendUserList(Server *server, User *user, Channel *channel, std::string const &chan_name) {
    if (!server->getChannel(chan_name))
        return;
    std::string serv_name = server->getName();
    std::list<User *> members = channel->getRegularMembers();
    std::list<User *> chops = channel->getOperators();
    for (std::list<User *>::iterator it = chops.begin(); it != chops.end(); it++) {
        channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), "@" + (*it)->getNickname()));
    }
    for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++) {
        channel->broadcast(server, RPL_NAMREPLY(user->getNickname(), channel->getName(), (*it)->getNickname()));
    }
    channel->broadcast(server, RPL_ENDOFNAMES(user->getNickname(), channel->getName()));
}

/**
 * @brief Check if a channel name is valid
 * @param name The name to check
 * @return bool True if the name is valid, false otherwise
 */
bool isChanNameValid(std::string const &name) {
    if (name.empty() || name.size() > CHANNELLEN)
        return false;
    if (name[0] != '#' && name[0] != '&')
        return false;
    for (size_t i = 0; i < name.size(); i++) {
        if (name[i] == ' ' || name[i] == ',' || name[i] == '\r' || name[i] == '\n' || name[i] == '\t')
            return false;
        if (i > 0 && name[i] == 'G' && name[i - 1] == '^')
            return false;
    }
    return true;
}

/**
 * @brief Check if a nickname is valid
 * @param nick The nickname to check
 * @return bool True if the nickname is valid, false otherwise
 */
bool is_nickname_valid(std::string const &nick) {
    if (nick.empty() || nick.size() > NICKLEN)
        return false;
    if (nick[0] == ':' || nick[0] == '#' || nick[0] == '&' || nick[0] == '~' || nick[0] == '+' || nick[0] == '%')
        return false;
    if (nick == "bot")
        return false;
    for (size_t i = 0; i < nick.size(); i++) {
        if (nick[i] == ' ' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@' ||
            nick[i] == '\r' || nick[i] == '\n' || nick[i] == '\t')
            return false;
    }
    return true;
}
