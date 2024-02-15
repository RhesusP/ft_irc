/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:03:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/15 11:54:27 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP
# define BUFF_SIZE 512

#include <iostream>
#include <vector>
#include <queue>
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
#include "Exceptions.hpp"
#include "Message.hpp"
#include "Utils.hpp"


# define CORED "\033[0;31m"
# define COGRE "\033[0;32m"
# define CORES "\033[0m"
# define COYEL "\033[0;33m"


std::string	receve(int fd);

#endif
