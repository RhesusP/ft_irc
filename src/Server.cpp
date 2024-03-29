/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:45:40 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/21 11:59:44 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

const char *BadPortException::what() const throw() {
    return "Bad port";
}

const char *SocketFailedException::what() const throw() {
    return "Socket creation failed";
}

const char *SetsockoptFailedException::what() const throw() {
    return "Setsockopt failed";
}

const char *BindFailedException::what() const throw() {
    return "Binding port failed";
}

const char *ListenFailedException::what() const throw() {
    return "Listen failed";
}

const char *PollFailedException::what() const throw() {
    return "Poll failed";
}

const char *setSocketNonBlockingException::what() const throw() {
    return ("setSocketNonBlocking failed");
}

Server::Server(void) {
    _password = "";
    _port = 1234;
    _name = SERVER_NAME;
    _creation_time = time(NULL);
}

Server::Server(std::string port, std::string password) {
    this->setPort(port);
    this->_password = password;
    _name = SERVER_NAME;
    _creation_time = time(NULL);
}

Server::~Server(void) {}

std::ostream &operator<<(std::ostream &o, Server &rhs) {
    o << "Server " << rhs.getName() << " created at " << timestr(rhs.getCreationTime()) << std::endl;
    std::list<Channel *> channels = rhs.getChannels();
    for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
        o << **it << std::endl;
    }
    std::list<User *> users = rhs.getUsers();
    for (std::list<User *>::iterator it = users.begin(); it != users.end(); it++) {
        o << **it << std::endl;
    }
    return o;
}

/* ************************************************************************** */
/*                                GETTERS                                     */
/* ************************************************************************** */

int Server::getPort(void) const {
    return _port;
}

int Server::getSocket(void) const {
    return _servSocket;
}

std::string Server::getPassword(void) const {
    return _password;
}

time_t Server::getCreationTime(void) const {
    return _creation_time;
}

std::list<struct pollfd> Server::getClientsFds(void) {
    return _clients_fds;
}

std::string const &Server::getName(void) const {
    return _name;
}

std::list<Channel *> Server::getChannels(void) {
    std::list<Channel *> res;
    for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        res.push_back(&(*it));
    }
    return res;
}

Channel *Server::getChannel(std::string const &name) {
    for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (it->getName() == name)
            return &(*it);
    }
    return NULL;
}

std::list<User *> Server::getUsers(void) {
    std::list<User *> res;
    for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++) {
        res.push_back(&(*it));
    }
    return res;
}

User *Server::getUser(int fd) {
    for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++) {
        if (it->getFD() == fd)
            return &(*it);
    }
    return NULL;
}

User *Server::getUser(std::string const &nickname) {
    for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++) {
        if (it->getNickname() == nickname)
            return &(*it);
    }
    return NULL;
}

/* ************************************************************************** */
/*                                SETTERS                                     */
/* ************************************************************************** */

void Server::setPassword(std::string const &password) {
    this->_password = password;
}

/**
 * @brief Set the port of the server
 * @details Check if the port is valid and set it
 * @throws BadPortException if the port is invalid
 * @param port
 */
void Server::setPort(std::string const &port) {
    int p;
    size_t len = port.size();
    if (len < 4 || len > 6)
        throw BadPortException();
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(port[i]))
            throw BadPortException();
    }
    p = std::atoi(port.c_str());
    if (p < 1024 || p > 65535)
        throw BadPortException();
    this->_port = p;
}

/* ************************************************************************** */
/*                                    NETWORK                                 */
/* ************************************************************************** */

/**
 * @brief Initialize the server network
 * @details Create a socket, bind it to the port and listen for incoming connections
 * @throws SocketFailedException if the socket creation failed
 * @throws SetsockoptFailedException if the setsockopt failed
 * @throws BindFailedException if the binding failed
 * @throws ListenFailedException if the listen failed
 */
void Server::initNetwork(void) {
    int opt_val = 1;
    _servSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (_servSocket == -1)
        throw SocketFailedException();
    if (setsockopt(_servSocket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1)
        throw SetsockoptFailedException();
    struct pollfd ufd;
    ufd.fd = _servSocket;
    ufd.events = POLLIN;
    _clients_fds.push_back(ufd);
    setSocketNonBlocking(_servSocket);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&addr.sin_zero, '\0', 8);
    if (bind(_servSocket, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        close(_servSocket);
        throw BindFailedException();
    }
    PRINT_SUCCESS("Server initialized on port " << this->getPort() << " and password \"" << this->getPassword()
                                                << "\" linked to socket " << this->getSocket());
    if (listen(_servSocket, MAX_CON_QUEUE) == -1)
        throw ListenFailedException();
    PRINT_INFO("Server listening on port " << this->getPort());
    while (true) {
        waitingForClient();
    }
}

/**
 * @brief Wait for incoming connections
 * @details Poll the server socket and the clients sockets for incoming data
 * @throws PollFailedException if the poll failed
 */
void Server::waitingForClient(void) {
    for (std::list<pollfd>::iterator it = _clients_fds.begin(); it != _clients_fds.end(); it++) {
        _v_clients_fds.push_back(*it);
    }
    int ret = poll(&_v_clients_fds[0], _v_clients_fds.size(), -1);
    if (ret == -1)
        throw PollFailedException();
    for (size_t i = 0; i < _v_clients_fds.size(); i++) {
        if (_v_clients_fds[i].revents == 0)
            continue;
        if (_v_clients_fds[i].fd == _servSocket)
            acceptNewConnection();
        else if (i > 0) {
            std::list<User>::iterator it = _users.begin();
            std::advance(it, i - 1);
            readData(&(*it));
        }
    }
    _v_clients_fds.clear();
}

/**
 * @brief Set a socket to non-blocking mode
 * @param fd The socket to set to non-blocking mode
 */
void Server::setSocketNonBlocking(int fd) {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        PRINT_ERROR("Error: failed to set socket non-blocking");
        if (fd != _servSocket)
            removeUser(fd, "");
    }
}

/**
 * @brief Accept a new connection
 * @details Accept a new connection and add the user to the server
 */
void Server::acceptNewConnection(void) {
    int socket;
    sockaddr_in usr;
    socklen_t size = sizeof(sockaddr_in);
    // PRINT_INFO("New connection detected");
    do {
        socket = accept(_servSocket, (struct sockaddr *) &usr, &size);
        if (socket == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
                PRINT_ERROR("Error: failed to accept new connection");
            break;
        }
        addUser(socket, inet_ntoa(usr.sin_addr), htons(usr.sin_port));
    } while (socket != -1);
}

/**
 * @brief Read data from a user socket and process it
 * @details Read data from a user socket and process it by splitting it into messages and calling the appropriate command
 * @param user The user to read data from
 */
void Server::readData(User *user) {
    char buf[BUFF_SIZE];
    ssize_t size;
    int fd = user->getFD();
    do {
        size = recv(fd, buf, BUFF_SIZE, 0);
        if (size == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                PRINT_ERROR("Error: failed to read data from socket " << fd);
                removeUser(fd, "Failed to contact client");
            }
            break;
        } else if (size == 0) {
            std::cout << "User " << fd << " disconnected" << std::endl;
            removeUser(fd, "Client disconnected");
            break;
        } else {
            buf[size] = '\0';
            std::string data = buf;
            formatRecv(data, user);
        }
    } while (true);
}

/**
 * @brief Send data to a user
 * @details Send data to a user socket and log the message
 * @param sender The sender of the message
 * @param message The message to send
 * @param fd The socket to send the message to
 * @return The number of bytes sent
 */
ssize_t Server::sendData(std::string sender, std::string message, int fd) {
    std::time_t now = time(NULL);

    message = "@time=" + timestr(now) + " :" + sender + " " + message;
    if (message[message.size() - 1] != '\n' && message[message.size() - 2] != '\r')
        message += "\r\n";
    PRINT_SEND(fd, message);
    ssize_t size = send(fd, message.c_str(), message.size(), 0);
    if ((unsigned int) size != message.size())
        std::cerr << "Error: failed to send message to " << fd << std::endl;
    return size;
}

/**
 * @brief Format received data
 * @details Format received data by splitting it into messages and calling the appropriate command. The data is
 * stored in the user stash until a full message is received.
 * @param rec The received data
 * @param user The user who sent the data
 */
void Server::formatRecv(std::string rec, User *user) {
    std::string delimiter = "\n";
    std::string msg;
    size_t pos = 0;
    int fd = user->getFD();

    if (rec.size() > 512) {
        rec.resize(510);
    }
    rec = user->getStash() + rec;
    while ((pos = rec.find(delimiter)) != std::string::npos) {
        if (rec.size() > 1 && rec.at(pos - 1) == '\r')
            msg = rec.substr(0, pos - 1);
        else
            msg = rec.substr(0, pos);
        PRINT_RECEIVED(user->getFD(), msg);
        Message(this, user, msg);
        user = getUser(fd);
        if (!user)
            return;
        user->setStash("");
        rec.erase(0, pos + delimiter.length());
    }
    user->setStash(rec);
}

/**
 * @brief Shutdown the server
 * @details Close all client sockets and clear the server data
 */
void Server::shutdown(void) {
    for (std::list<struct pollfd>::iterator it = _clients_fds.begin(); it != _clients_fds.end(); it++) {
        std::cout << "Closing socket " << it->fd << std::endl;
        close(it->fd);
    }
    _clients_fds.clear();
    _v_clients_fds.clear();
}

/* ************************************************************************** */
/*                               USER MANAGEMENT                              */
/* ************************************************************************** */

/**
 * @brief Add a user to the server
 * @param socket
 * @param ip
 * @param port
 */
void Server::addUser(int socket, char *ip, int port) {
    User user(this, socket, ip, port);
    _users.push_back(user);
    setSocketNonBlocking(socket);
    struct pollfd ufd;
    ufd.fd = socket;
    ufd.events = POLLIN;
    _clients_fds.push_back(ufd);
}

/**
 * @brief Remove a user from the server
 * @details Remove a user from the server and handle the user's channels. If the user is the last user on a channel,
 * the channel is deleted. Else, a QUIT message is broadcasted to all users on the channel.
 * @param socket The socket of the user to remove
 * @param reason The reason for the user's removal
 */
void Server::removeUser(int socket, std::string const &reason) {
    User *user = getUser(socket);
    if (!user) {
        PRINT_ERROR("Error: failed to remove user from server");
        return;
    }
    std::list<Channel *> channels = user->getChannels();
    for (std::list<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
        PRINT_INFO("Removing user " << user->getNickname() << " from channel " << (*it)->getName());
        (*it)->broadcast(user, RPL_QUIT(reason));
        if (this->getChannel((*it)->getName()) && (*it)->isBotActivated())
            CmdBot(this).goodbye(*it, user);
        (*it)->removeUser(user);
    }
    for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++) {
        if (*user == (*it)) {
            PRINT_INFO("Removing user " << user->getNickname() << " from server");
            _users.remove(*it);
            break;
        }
    }
    for (std::list<struct pollfd>::iterator it = _clients_fds.begin(); it != _clients_fds.end(); it++) {
        if (it->fd == socket) {
            PRINT_INFO("Removing socket " << socket << " from server");
            _clients_fds.erase(it);
            break;
        }
    }
    close(socket);
}

/* ************************************************************************** */
/*                              CHANNEL MANAGEMENT                            */
/* ************************************************************************** */

/**
 * @brief Add a channel to the server
 * @details Add a channel to the server and link it to the founder
 * @param channel The channel to add
 * @param founder The founder of the channel
 * @return The added channel
 */
Channel *Server::addChannel(Channel channel, User *founder) {
    if (getChannel(channel.getName()) != NULL) {
        PRINT_ERROR("Error: channel " + channel.getName() + " already exists");
        return NULL;
    }
    _channels.push_back(channel);
    founder->addinChannel(&(_channels.back()));
    PRINT_SUCCESS("Channel " + channel.getName() + " has been added to server");
    return &(_channels.back());
}

/**
 * @brief Remove a channel from the server
 * @param channel The channel to remove
 */
void Server::removeChannel(Channel channel) {
    for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (*it == channel) {
            _channels.erase(it);
            break;
        }
    }
    PRINT_SUCCESS("Channel " + channel.getName() + " has been deleted from server");
}
