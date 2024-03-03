/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:25:41 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/03 00:16:39 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdMotd::CmdMotd(Server *server)
{
	_server = server;
	_need_auth = true;
	_need_registration = true;
}

CmdMotd::~CmdMotd(void){}

void CmdMotd::execute(Message *message)
{
	User *user = message->getAuthor();
	int fd = user->getFD();
	std::string serv_name = _server->getName();


	this->reply(serv_name, RPL_MOTDSTART(user->getNickname()), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "d8b                                                            "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "Y8P                                                            "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "                                                               "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "888 888d888 .d8888b         .d8888b   .d88b.  888d888 888  888 "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "888 888P\"  d88P\"            88K      d8P  Y8b 888P\"   888  888 "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "888 888    888              \"Y8888b. 88888888 888     Y88  88P "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "888 888    Y88b.                 X88 Y8b.     888      Y8bd8P  "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "888 888     \"Y8888P 88888888 88888P'  \"Y8888  888       Y88P  "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "                                                               "), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "                                                       cbernot"), fd);
	this->reply(serv_name, RPL_MOTD(user->getNickname(), "                                                      svanmeen"), fd);
	this->reply(serv_name, RPL_ENDOFMOTD(user->getNickname()), fd);
}

// d8b                                                            
// Y8P                                                            
                                                               
// 888 888d888 .d8888b         .d8888b   .d88b.  888d888 888  888 
// 888 888P"  d88P"            88K      d8P  Y8b 888P"   888  888 
// 888 888    888              "Y8888b. 88888888 888     Y88  88P 
// 888 888    Y88b.                 X88 Y8b.     888      Y8bd8P  
// 888 888     "Y8888P 88888888 88888P'  "Y8888  888       Y88P  
                                                               
//                                                        cbernot
//                                                       svanmeen