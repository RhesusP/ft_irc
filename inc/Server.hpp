/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:34:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/20 23:13:38 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"

class User;

class Channel;

class Message;

class setSocketNonBlockingException : public std::exception {
public:
    virtual const char *what() const throw();
};

class BadPortException : public std::exception {
public:
    virtual const char *what() const throw();
};

class SocketFailedException : public std::exception {
public:
    virtual const char *what() const throw();
};

class SetsockoptFailedException : public std::exception {
public:
    virtual const char *what() const throw();
};

class BindFailedException : public std::exception {
public:
    virtual const char *what() const throw();
};

class ListenFailedException : public std::exception {
public:
    virtual const char *what() const throw();
};

class PollFailedException : public std::exception {
public:
    virtual const char *what() const throw();
};

class Server {
private:
    int _servSocket;
    int _port;
    std::string _password;
    std::string _name;
    std::list<Channel> _channels;
    std::list<User> _users;
    std::list<struct pollfd> _clients_fds;
    std::vector<struct pollfd> _v_clients_fds;
    std::time_t _creation_time;

    void formatRecv(std::string rec, User *usr);

    void setSocketNonBlocking(int fd);

    void waitingForClient(void);

    void acceptNewConnection(void);

    void readData(User *User);

public:
    Server(void);

    Server(std::string port, std::string password);

    ~Server(void);

    int getPort(void) const;

    std::string const &getName(void) const;

    int getSocket(void) const;

    std::string getPassword(void) const;

    std::list<User *> getUsers(void);

    time_t getCreationTime(void) const;

    std::list<struct pollfd> getClientsFds(void);

    void setPassword(std::string const &port);

    void setPort(std::string const &port);

    User *getUser(std::string const &nickname);

    User *getUser(int fd);

    void addUser(int socket, char *ip, int port);

    void removeUser(int socket, std::string const &reason);

    std::list<Channel *> getChannels(void);

    Channel *getChannel(std::string const &name);

    Channel *addChannel(Channel channel, User *founder);

    void removeChannel(Channel channel);

    void initNetwork(void);

    ssize_t sendData(std::string sender, std::string message, int fd);

    void shutdown(void);
};

std::ostream &operator<<(std::ostream &o, Server &rhs);

#endif
