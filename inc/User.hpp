/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:44:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/11 12:37:01 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "ircserv.hpp"

class Channel;

class Server;

class User {
private:
    Server *_server;
    int _fd;
    int _port;
    bool _isAuth;
    std::string _username;
    std::string _realname;
    std::string _nickname;
    std::string _hostname;
    std::list<Channel *> _channels;
    std::list<Channel *> _invite_list;
    std::string _stash;

public:
    User(void);

    User(Server *server, int fd, std::string const &hostname, int port);

    ~User(void);

    std::string const &getNickname(void) const;

    std::string const &getUsername(void) const;

    std::string const &getRealname(void) const;

    std::string const &getHostname(void) const;

    std::string getIdentity(void) const;

    std::list<Channel *> getChannels(void) const;

    int getFD(void) const;

    bool getIsAuth(void) const;

    bool isRegistered(void) const;

    std::string const &getStash();

    void setNickname(std::string const &nickname);

    void setUsername(std::string const &username);

    void setRealname(std::string const &realname);

    void setIsAuth(bool auth);

    void setStash(std::string const &stash);

    void addinChannel(Channel *chan);

    void removefromChannel(Channel *chan);

    bool isOnChannel(Channel *chan);


    void inviteToChannel(Channel *chan);

    void removeFromInviteList(Channel *chan);

    std::list<Channel *> getInviteList(void);

    bool isInvitedTo(Channel *chan);

    bool operator==(User const &rhs) const;
};

std::ostream &operator<<(std::ostream &o, User const &rhs);

#endif
