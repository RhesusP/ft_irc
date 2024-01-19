/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 02:03:44 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/19 16:08:30 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP
# define BUFF_SIZE 512

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <stdlib.h>
#include <signal.h>
#include <cstring>
# include <poll.h>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"

#endif
