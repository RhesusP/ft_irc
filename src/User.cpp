/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:49 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/11 12:38:06 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void) {
    _fd = -1;
    _port = -1;
    _username = "";
    _nickname = "";
    _realname = "";
    _hostname = "";
    _isAuth = false;
}

User::User(Server *server, int fd, std::string const &hostname, int port) {
    std::cout << "User " << fd << " | " << hostname << " created" << std::endl;
    _server = server;
    _fd = fd;
    _hostname = hostname;
    _port = port;
    _username = "";
    _nickname = "";
    _realname = "";
    _isAuth = false;
}

User::~User(void) {}

bool User::operator==(User const &rhs) const {
    return (_fd == rhs.getFD() && _nickname == rhs.getNickname());
}

std::ostream &operator<<(std::ostream &o, User const &rhs) {
    o << "------------------------------" << std::endl;
    o << "User (" << rhs.getFD() << ") " << std::endl;
    o << "\tNickname: " << rhs.getNickname() << std::endl;
    o << "\tUsername: " << rhs.getUsername() << std::endl;
    o << "\tRealname: " << rhs.getRealname() << std::endl;
    o << "\tHostname: " << rhs.getHostname() << std::endl;
    o << "\tIsAuth: " << (rhs.getIsAuth() ? "true" : "false") << std::endl;
    o << "\tChannels: " << std::endl;
    std::list<Channel *> channels = rhs.getChannels();
    for (std::list<Channel *>::const_iterator it = channels.begin(); it != channels.end(); it++) {
        o << "\t\t" << (*it)->getName() << std::endl;
    }
    o << "------------------------------" << std::endl;
    return o;
}

/* ************************************************************************** */
/*                                GETTERS                                     */
/* ************************************************************************** */

int User::getFD(void) const {
    return _fd;
}

std::string const &User::getNickname(void) const {
    return _nickname;
}

bool User::getIsAuth(void) const {
    return _isAuth;
}

std::string const &User::getHostname(void) const {
    return _hostname;
}

std::string const &User::getUsername(void) const {
    return _username;
}

std::string const &User::getStash(void) {
    return _stash;
}

std::string const &User::getRealname(void) const {
    return _realname;
}

std::string User::getIdentity(void) const {
    std::string nick, user, host;

    nick = _nickname.empty() ? "*" : _nickname;
    user = _username.empty() ? "*" : _username;
    host = _hostname.empty() ? "*" : _hostname;
    return (nick + "!" + user + "@" + host);
}

std::list<Channel *> User::getChannels(void) const {
    return _channels;
}

bool User::isRegistered(void) const {
    if (!_nickname.empty() && !_username.empty() && !_realname.empty())
        return true;
    return false;
}

std::list<Channel *> User::getInviteList(void) {
    return _invite_list;
}

/* ************************************************************************** */
/*                                SETTERS                                     */
/* ************************************************************************** */

void User::setIsAuth(bool auth) {
    _isAuth = auth;
}

void User::setStash(std::string const &stash) {
    _stash = stash;
}

void User::setUsername(std::string const &username) {
    _username = username;
}

void User::setNickname(std::string const &nickname) {
    _nickname = nickname;
}

void User::setRealname(std::string const &realname) {
    _realname = realname;
}

/* ************************************************************************** */
/*                           CHANNEL MANAGEMENT                               */
/* ************************************************************************** */

bool User::isOnChannel(Channel *chan) {
    for (std::list<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (*it == chan)
            return true;
    }
    return false;
}

void User::addinChannel(Channel *chan) {
    _channels.push_back(chan);
}

void User::removefromChannel(Channel *chan) {
    for (std::list<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (*it == chan) {
            _channels.erase(it);
            break;
        }
    }
}

bool User::isInvitedTo(Channel *channel) {
    for (std::list<Channel *>::iterator it = _invite_list.begin(); it != _invite_list.end(); it++) {
        if (*it == channel)
            return true;
    }
    return false;
}

void User::inviteToChannel(Channel *chan) {
    _invite_list.push_back(chan);
}

void User::removeFromInviteList(Channel *chan) {
    for (std::list<Channel *>::iterator it = _invite_list.begin(); it != _invite_list.end(); it++) {
        if (*it == chan) {
            _invite_list.erase(it);
            break;
        }
    }
}
