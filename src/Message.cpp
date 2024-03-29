/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/18 23:21:06 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

Message::Message(void) {}

Message::Message(Server *server, User *user, std::string const &raw) {
    _server = server;
    _raw = raw;
    _raw = getTags(_raw);
    _raw = getSource(_raw);
    _raw = getCommand(_raw);
    getParameters(_raw);
    _author = user;
    processMessage();
}

Message::~Message(void) {}

std::ostream &operator<<(std::ostream &o, Message &rhs) {
    std::map<std::string, std::string> tags = rhs.getTags();
    std::string source = rhs.getSource();
    std::vector<std::string> params = rhs.getParameters();

    std::cout << "============ NEW MESSAGE ============" << std::endl;
    std::cout << "\tTags: " << std::endl;
    for (std::map<std::string, std::string>::iterator it = tags.begin(); it != tags.end(); it++)
        std::cout << "\t\t" << it->first << "=" << it->second << std::endl;
    std::cout << "\tSource: " << source << std::endl;
    std::cout << "\tCommand: " << rhs.getCommand() << std::endl;
    std::cout << "\tParameters: " << std::endl;
    for (size_t i = 0; i < params.size(); i++)
        std::cout << "\t\t'" << params[i] << "'" << std::endl;
    return o;
}

/* ************************************************************************** */
/*                                GETTERS                                     */
/* ************************************************************************** */

std::map<std::string, std::string> &Message::getTags(void) {
    return _tags;
}

std::string const &Message::getSource(void) {
    return _source;
}

std::string const &Message::getCommand(void) const {
    return _command;
}

std::vector<std::string> Message::getParameters(void) {
    return _parameters;
}

User *Message::getAuthor(void) const {
    return _author;
}

/* ************************************************************************** */
/*                                 PARSER                                     */
/* ************************************************************************** */

/**
 * @brief Extracts the tags from the raw message and store them in the _tags attribute
 * @param raw The raw message
 * @return The raw message without the tags
 */
std::string Message::getTags(std::string const &raw) {
    if (raw[0] != '@')
        return raw;
    int endpos = raw.find(' ');
    std::string str = raw.substr(1, endpos - 1);
    std::vector<std::string> tags_tab = ::split(str, ";");
    std::vector<std::string> tag_tab;
    for (std::vector<std::string>::iterator it = tags_tab.begin(); it != tags_tab.end(); it++) {
        tag_tab = ::split(*it, "=");
        if (tag_tab.size() > 2) {
            std::cerr << "Error: invalid tag" << std::endl;
            return "";
        }
        std::vector<std::string>::iterator ita = tag_tab.begin();
        std::string key = *ita;
        ++ita;
        std::string value;
        if (ita == tag_tab.end())
            value = "";
        else
            value = *ita;
        _tags.insert(std::pair<std::string, std::string>(key, value));
        tag_tab.clear();
    }
    std::string rawCopy = raw;
    rawCopy.erase(0, endpos + 1);
    return rawCopy;
}

/**
 * @brief Extracts the source from the raw message and store it in the _source attribute
 * @param raw The raw message
 * @return The raw message without the source
 */
std::string Message::getSource(std::string const &raw) {
    std::string str = trim(raw, " ");
    if (str[0] != ':')
        return str;
    int endpos = str.find(' ');
    _source = str.substr(1, endpos - 1);
    std::string rawCopy = str;
    rawCopy.erase(0, endpos + 1);
    return rawCopy;
}

/**
 * @brief Extracts the command from the raw message and store it in the _command attribute
 * @param raw The raw message
 * @return The raw message without the command
 */
std::string Message::getCommand(std::string const &raw) {
    std::string str = trim(raw, " ");
    std::string s;
    size_t endpos = str.find(' ');
    if (endpos != std::string::npos)
        s = str.substr(0, endpos);
    else
        s = str;
    _command = s;
    std::string rawCopy = str;
    if (endpos != std::string::npos)
        rawCopy.erase(0, endpos + 1);
    else
        rawCopy.clear();
    return rawCopy;
}

/**
 * @brief Extracts the parameters from the raw message and store them in the _parameters attribute
 * @param raw The raw message
 */
void Message::getParameters(std::string const &raw) {
    std::vector<std::string> tab = split(raw, " ");
    std::vector<std::string> new_tab;
    for (size_t i = 0; i < tab.size(); i++) {
        if (tab[i][0] == ':') {
            std::string remain = tab[i];
            for (size_t j = i + 1; j < tab.size(); j++) {
                remain.append(" " + tab[j]);
            }
            new_tab.push_back(remain);
            break;
        } else {
            new_tab.push_back(tab[i]);
        }
    }
    for (size_t i = 0; i < new_tab.size(); i++) {
        if (new_tab[i][0] == ':') {
            new_tab[i].erase(0, 1);
        }
    }
    _parameters = new_tab;
}

/* ************************************************************************** */
/*                                 PROCESS                                    */
/* ************************************************************************** */

/**
 * @brief Processes the message by executing the corresponding command
 * @details The command is executed if it is known, otherwise an error is returned. The command is executed only if the
 * user is authenticated and registered if needed. CAP command is not processed.
 */
void Message::processMessage(void) {
    std::string cmds_name[NBCMDS] = {"MOTD", "NICK", "PASS", "PING", "QUIT", "UNKNOWN", "USER", "JOIN", "PART", "MODE",
                                     "TOPIC", "KICK", "INVITE", "PRIVMSG", "WHO", "BOT"};
    CmdMotd cmdMotd(_server);
    CmdNick cmdNick(_server);
    CmdPass cmdPass(_server);
    CmdPing cmdPing(_server);
    CmdQuit cmdQuit(_server);
    CmdUnknown mdUnknown(_server);
    CmdUser cmdUser(_server);
    CmdJoin CmdJoin(_server);
    CmdPart CmdPart(_server);
    CmdMode CmdMode(_server);
    CmdTopic CmdTopic(_server);
    CmdKick CmdKick(_server);
    CmdInvite CmdInvite(_server);
    CmdPrivmsg CmdPrivmsg(_server);
    CmdWho CmdWho(_server);
    CmdBot CmdBot(_server);

    Command *cmds[NBCMDS] = {
            &cmdMotd,
            &cmdNick,
            &cmdPass,
            &cmdPing,
            &cmdQuit,
            &mdUnknown,
            &cmdUser,
            &CmdJoin,
            &CmdPart,
            &CmdMode,
            &CmdTopic,
            &CmdKick,
            &CmdInvite,
            &CmdPrivmsg,
            &CmdWho,
            &CmdBot
    };
    for (int i = 0; i < NBCMDS; i++) {
        if (toupper(this->_command) == cmds_name[i]) {
            if ((cmds[i]->getNeedAuth() && !_author->getIsAuth()) ||
                (cmds[i]->getNeedRegistration() && !_author->isRegistered())) {
                _server->sendData(_server->getName(), ERR_NOTREGISTERED(_author->getNickname()), _author->getFD());
                return;
            }
            cmds[i]->execute(this);
            return;
        }
    }
    if (this->_command != "CAP") {
        CmdUnknown cmdUnknown(_server);
        cmdUnknown.execute(this);
    }
}
