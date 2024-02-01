/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:03:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/01 14:29:01 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

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
#include <sstream>
#include <map>
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include "Message.hpp"
#include "Utils.hpp"

#endif