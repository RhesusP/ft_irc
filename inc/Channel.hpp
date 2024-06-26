/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:43:33 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/18 23:19:35 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ircserv.hpp"

class User;

class Server;

class Channel {
private:
    Server *_server;
    std::list<User *> _members;
    std::list<User *> _operators;
    std::string _name;
    std::string _topic;
    std::string _key;
    int _limit;
    bool _is_invite_only;
    bool _is_topic_restricted;
    std::time_t _creation_time;
    bool _is_bot_activated;

public:
    Channel(Server *server);

    Channel(Server *server, std::string const &name, User *founder);

    Channel &operator=(Channel &rhs);

    bool operator==(Channel const &rhs);

    ~Channel(void);

    std::string const &getName(void) const;

    std::string const &getTopic(void) const;

    std::string const &getKey(void) const;

    std::time_t getCreationTime(void) const;

    std::list<User *> getRegularMembers(void);

    std::list<User *> getOperators(void);

    size_t nbMembers(void) const;

    int getLimit(void) const;

    bool isInviteOnly(void) const;

    bool isTopicRestricted(void) const;

    std::string getModes(void) const;

    void setTopic(std::string const &topic);

    void setKey(std::string const &key);

    void setLimit(int limit);

    void setInviteOnly(bool invite_only);

    void setTopicRestricted(bool topic_restricted);

    // Member management
    bool addRegularMember(User *user);

    bool removeRegularMember(User *user);

    void removeUser(User *user);

    void addOperator(User *user);

    bool removeOperator(User *user);

    bool isInChannel(User *user);

    bool isRegularMember(User *user);

    bool isOperator(User *user);

    bool isOperator(std::string const &nickname);

    void addMode(char mode, std::string const &arg, User *sender);

    void removeMode(char mode, std::string const &arg, User *sender);

    void broadcast(User *sender, std::string const &message);

    void broadcast(Server *server, std::string const &message);

    void broadcast(std::string const &sender, std::string const &message);

    bool isBotActivated(void) const;

    void setBotActivated(bool activated);
};

std::ostream &operator<<(std::ostream &o, Channel &rhs);

#endif
