/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:23 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 12:14:00 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(Server *server) {
    _server = server;
    _name = "";
    _topic = "";
    _key = "";
    _limit = -1; // no limit
    _is_invite_only = false;
    _is_topic_restricted = false;
    _is_bot_activated = false;
    _creation_time = time(NULL);
}

Channel::Channel(Server *server, std::string const &name, User *founder) {
    _server = server;
    _name = name;
    _topic = "";
    _key = "";
    _limit = -1; // no limit
    _is_invite_only = false;
    _is_topic_restricted = false;
    _is_bot_activated = false;
    _operators.push_back(founder);
    _creation_time = time(NULL);
}

Channel::~Channel(void) {}

bool Channel::operator==(Channel const &rhs) {
    return _name == rhs.getName();
}

Channel &Channel::operator=(Channel &rhs) {
    _server = rhs._server;
    _members = rhs.getRegularMembers();
    _operators = rhs.getOperators();
    _name = rhs.getName();
    _topic = rhs.getTopic();
    _key = rhs.getKey();
    _limit = rhs.getLimit();
    _is_invite_only = rhs.isInviteOnly();

    return *this;
}

std::ostream &operator<<(std::ostream &o, Channel &rhs) {
    o << "------------------------------------------" << std::endl;
    o << "Channel " << rhs.getName() << std::endl;
    o << "Topic: " << rhs.getTopic() << std::endl;
    o << "Key: " << rhs.getKey() << std::endl;
    o << "Limit: " << rhs.getLimit() << std::endl;
    o << "Mode: " << rhs.getModes() << std::endl;
    o << "Members: " << std::endl;
    std::list<User *> members = rhs.getRegularMembers();
    for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++) {
        o << "\t"
          << "(" << (*it)->getFD() << ") " << (*it)->getNickname() << std::endl;
    }
    o << "Operators: " << std::endl;
    std::list<User *> operators = rhs.getOperators();
    for (std::list<User *>::iterator it = operators.begin(); it != operators.end(); it++) {
        o << "\t"
          << "(" << (*it)->getFD() << ") " << (*it)->getNickname() << std::endl;
    }
    o << "------------------------------------------" << std::endl;
    return o;
}

/* ************************************************************************** */
/*                                GETTERS                                     */
/* ************************************************************************** */

size_t Channel::nbMembers(void) const {
    return _members.size() + _operators.size();
}

std::string const &Channel::getName(void) const {
    return _name;
}

std::string const &Channel::getTopic(void) const {
    return _topic;
}

std::string const &Channel::getKey(void) const {
    return _key;
}

int Channel::getLimit(void) const {
    return _limit;
}

bool Channel::isInviteOnly(void) const {
    return _is_invite_only;
}

bool Channel::isTopicRestricted(void) const {
    return _is_topic_restricted;
}

std::string Channel::getModes(void) const {
    std::string res = "";
    if (_limit != -1)
        res += "+l";
    if (_is_invite_only)
        res += "i";
    if (_is_topic_restricted)
        res += "t";
    if (_key.size() > 0)
        res += "k";
    return res;
}

std::time_t Channel::getCreationTime(void) const {
    return _creation_time;
}

bool Channel::isBotActivated(void) const {
    return _is_bot_activated;
}

/* ************************************************************************** */
/*                                SETTERS                                     */
/* ************************************************************************** */

std::list<User *> Channel::getRegularMembers(void) {
    return _members;
}

std::list<User *> Channel::getOperators(void) {
    return _operators;
}

void Channel::setTopic(std::string const &topic) {
    _topic = topic;
}

void Channel::setKey(std::string const &key) {
    _key = key;
}

void Channel::setLimit(int limit) {
    _limit = limit;
}

void Channel::setInviteOnly(bool is_invite_only) {
    _is_invite_only = is_invite_only;
}

void Channel::setTopicRestricted(bool is_topic_restricted) {
    _is_topic_restricted = is_topic_restricted;
}

void Channel::setBotActivated(bool is_bot_activated) {
    _is_bot_activated = is_bot_activated;
}

/* ************************************************************************** */
/*                              USER MANAGEMENT                               */
/* ************************************************************************** */

/**
 * @brief Check if a user is in the channel
 * @param user The user to check
 * @return true if the user is in the channel, false otherwise
 */
bool Channel::isInChannel(User *user) {
    if (isRegularMember(user) || isOperator(user))
        return true;
    return false;
}

/**
 * @brief Check if a user is a regular member of the channel
 * @param user The user to check
 * @return true if the user is a regular member, false otherwise
 */
bool Channel::isRegularMember(User *user) {
    for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
        if (*it == user)
            return true;
    }
    return false;
}

/**
 * @brief Check if a user is an operator of the channel
 * @param nickname The nickname of the user to check
 * @return true if the user is an operator, false otherwise
 */
bool Channel::isOperator(User *user) {
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        if (*it == user)
            return true;
    }
    return false;
}

/**
 * @brief Check if a user is an operator of the channel
 * @param nickname The nickname of the user to check
 * @return true if the user is an operator, false otherwise
 */
bool Channel::isOperator(std::string const &nickname) {
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return true;
    }
    return false;
}

/**
 * @brief Add a regular user to the channel
 * @param user The user to add
 * @return true if the user was added, false otherwise
 */
bool Channel::addRegularMember(User *user) {
    if (isInChannel(user)) {
        return (false);
    }
    _members.push_back(user);
    user->addinChannel(this);
    return (true);
}

/**
 * @brief Add an operator to the channel
 * @param user The user to add
 */
void Channel::addOperator(User *user) {
    if (isRegularMember(user)) {
        for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
            if (*it == user) {
                _members.erase(it);
                break;
            }
        }
        _operators.push_back(user);
        PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
    } else {
        PRINT_INFO("User " + user->getNickname() + " added to operators of channel " + _name);
        _operators.push_back(user);
        user->addinChannel(this);
    }
}

/**
 * @brief Remove a regular user from the channel
 * @param user The user to remove (must be a regular member)
 * @return true if the user was removed, false otherwise
 */
bool Channel::removeRegularMember(User *user) {
    bool ret = false;
    for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
        if (*it == user) {
            PRINT_INFO("Removing " + user->getNickname() + " from channel " + _name);
            _members.erase(it);
            user->removefromChannel(this);
            ret = true;
            break;
        }
    }
    if (_operators.size() == 0 && _members.size() == 0) {
        PRINT_INFO("No more members in channel " + _name + ", deleting it");
        _server->removeChannel(*this);
    }
    return ret;
}

/**
 * @brief Remove an operator from the channel
 * @param user The user to remove (must be an operator)
 * @return true if the user was removed, false otherwise
 */
bool Channel::removeOperator(User *user) {
    PRINT_INFO("Removing " + user->getNickname() + " from operators of channel " + _name);
    bool ret = false;
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        if (*(*it) == *user) {
            it = _operators.erase(it);
            user->removefromChannel(this);
            ret = true;
            break;
        }
    }
    if (_operators.empty() && !_members.empty()) {
        User *new_operator = *_members.begin();
        _operators.push_back(new_operator);
        _members.erase(_members.begin());
        PRINT_INFO(
                "No more operators in channel " + _name + ", setting " + new_operator->getNickname() + " as operator");
    } else if (_operators.empty() && _members.empty()) {
        PRINT_INFO("No more members in channel " + _name + ", deleting it");
        _server->removeChannel(*this);
    }
    return ret;
}

/**
 * @brief Remove a user from the channel
 * @param user The user to remove
 */
void Channel::removeUser(User *user) {
    if (!removeOperator(user))
        removeRegularMember(user);
}

/* ************************************************************************** */
/*                              MODE MANAGEMENT                               */
/* ************************************************************************** */

/**
 * @brief Add a mode to the channel
 * @details The mode must be one of the following characters: 'l', 't', 'k', 'o', 'i'.
 * If the mode is not valid, an error is sent to the user.
 * @param mode The mode to add
 * @param arg The argument of the mode
 * @param sender The user who sent the mode command
 */
void Channel::addMode(char mode, std::string const &arg, User *sender) {
    int limit;
    User *target = NULL;
    std::list<User *> lst;
    switch (mode) {
        case 'l':
            limit = atoi(arg.c_str());
            if (arg.size() > 10 || limit < 1) {
                _server->sendData(_server->getName(),
                                  ERR_INVALIDMODEPARAM(sender->getNickname(), this->getName(), "MODE +l", arg,
                                                       "limit is not valid"), sender->getFD());
                return;
            }
            this->setLimit(limit);
            this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+l " + arg));
            _server->sendData(sender->getIdentity(),
                              RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+l " + arg), sender->getFD());
            break;
        case 't':
            this->setTopicRestricted(true);
            this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+t"));
            _server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+t"),
                              sender->getFD());
            break;
        case 'k':
            if (arg.size() > 0) {
                this->setKey(arg);
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+k"));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+k"), sender->getFD());
            } else {
                _server->sendData(_server->getName(),
                                  ERR_INVALIDMODEPARAM(sender->getNickname(), this->getName(), "MODE +k", "",
                                                       "key is missing"), sender->getFD());
                return;
            }
            break;
        case 'o':
            lst = this->getRegularMembers();
            for (std::list<User *>::iterator it = lst.begin(); it != lst.end(); it++) {
                if ((*it)->getNickname() == arg) {
                    target = *it;
                    break;
                }
            }
            if (target) {
                this->addOperator(target);
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+o " + arg));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+o " + arg),
                                  sender->getFD());
            } else {
                if (!this->isOperator(arg))
                    _server->sendData(_server->getName(), ERR_NOTONCHANNEL(sender->getNickname(), this->getName()),
                                      sender->getFD());
            }
            break;
        case 'i':
            this->setInviteOnly(true);
            this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+i"));
            _server->sendData(sender->getIdentity(), RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "+i"),
                              sender->getFD());
            break;
        default:
            _server->sendData(_server->getName(), ERR_UNKNOWNMODE(sender->getNickname(), mode), sender->getFD());
            break;
    }
}

/**
 * @brief Remove a mode from the channel
 * @details The mode must be one of the following characters: 'l', 't', 'k', 'o', 'i'.
 * If the mode is not valid, an error is sent to the user.
 * @param mode The mode to remove
 * @param arg The argument of the mode
 * @param sender The user who sent the mode command
 */
void Channel::removeMode(char mode, std::string const &arg, User *sender) {
    switch (mode) {
        case 'l':
            if (this->getLimit() != -1) {
                this->setLimit(-1);
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-l"));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-l"), sender->getFD());
            }
            break;
        case 't':
            if (this->isTopicRestricted()) {
                this->setTopicRestricted(false);
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-t"));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-t"), sender->getFD());
            }
            break;
        case 'k':
            if (this->getKey().size() > 0) {
                this->setKey("");
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-k"));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-k"), sender->getFD());
            }
            break;
        case 'o':
            if (this->isOperator(arg)) {
                if (_operators.size() > 1) {
                    this->removeOperator(_server->getUser(arg));
                    this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-o " + arg));
                    _server->sendData(sender->getIdentity(),
                                      RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-o " + arg),
                                      sender->getFD());
                } else {
                    _server->sendData(_server->getName(),
                                      ERR_INVALIDMODEPARAM(sender->getNickname(), this->getName(), "MODE -o", arg,
                                                           "You cannot remove your operator privileges if you are the only operator of the channel"),
                                      sender->getFD());
                }
            } else {
                if (!this->isRegularMember(_server->getUser(arg)))
                    _server->sendData(_server->getName(), ERR_NOTONCHANNEL(sender->getNickname(), this->getName()),
                                      sender->getFD());
            }
            break;
        case 'i':
            if (this->isInviteOnly()) {
                this->setInviteOnly(false);
                this->broadcast(sender, RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-i"));
                _server->sendData(sender->getIdentity(),
                                  RPL_CHANNELMODEIS(sender->getNickname(), this->getName(), "-i"), sender->getFD());
            }
            break;
        default:
            _server->sendData(_server->getName(), ERR_UNKNOWNMODE(sender->getNickname(), mode), sender->getFD());
            break;
    }
}

/* ************************************************************************** */
/*                                BROADCASTING                                */
/* ************************************************************************** */

/**
 * @brief Broadcast a message to all users in the channel
 * @param sender The user who sent the message
 * @param message The message to broadcast
 */
void Channel::broadcast(User *sender, std::string const &message) {
    for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
        if (*it != sender)
            _server->sendData(sender->getIdentity(), message, (*it)->getFD());
    }
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        if (*it != sender)
            _server->sendData(sender->getIdentity(), message, (*it)->getFD());
    }
}

/**
 * @brief Broadcast a message to all users in the channel
 * @param server The server
 * @param message The message to broadcast
 */
void Channel::broadcast(Server *server, std::string const &message) {
    for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
        _server->sendData(server->getName(), message, (*it)->getFD());
    }
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        _server->sendData(server->getName(), message, (*it)->getFD());
    }
}

/**
 * @brief Broadcast a message to all users in the channel
 * @param sender The nickname of the user who sent the message
 * @param message The message to broadcast
 */
void Channel::broadcast(std::string const &sender, std::string const &message) {
    for (std::list<User *>::iterator it = _members.begin(); it != _members.end(); it++) {
        _server->sendData(sender, message, (*it)->getFD());
    }
    for (std::list<User *>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        _server->sendData(sender, message, (*it)->getFD());
    }
}
