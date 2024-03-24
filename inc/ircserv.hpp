/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:03:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 22:50:56 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP
#define BUFF_SIZE 512

#define INFO "\033[94m"
#define SUCCESS "\033[32m"
#define WARNING "\033[33m"
#define ERROR "\033[31m"
#define RESET "\033[39m"
#define RECEIVE "\033[35m"
#define SEND "\033[36m"

#define PRINT_INFO(msg) std::cout << INFO << msg << RESET << std::endl
#define PRINT_SUCCESS(msg) std::cout << SUCCESS << msg << RESET << std::endl
#define PRINT_WARNING(msg) std::cout << WARNING << msg << RESET << std::endl
#define PRINT_ERROR(msg) std::cout << ERROR << msg << RESET << std::endl
#define PRINT_RECEIVED(from, msg) std::cout << RECEIVE << "◀ (" << from << ") " << msg << RESET << std::endl
#define PRINT_SEND(to, msg) std::cout << SEND << "▶ (" << to << ") " << msg << RESET << std::endl

#define SERVER_NAME "ircserv"
#define MAX_CON_QUEUE 20
#define CHANNELLEN 64
#define NICKLEN 32
#define TARGMAX 20
#define TOPICLEN 307
#define USERLEN 18
#define NBCMDS 16

#include <iostream>
#include <cerrno>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <stdlib.h>
#include <signal.h>
#include <cstring>
#include <poll.h>
#include "fcntl.h"
#include "unistd.h"
#include <ctime>
#include <sstream>
#include <map>
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Utils.hpp"
#include "Commands.hpp"
#include "Replies.hpp"

#endif
