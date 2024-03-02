/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:36:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/02 17:59:42 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include "ircserv.hpp"

class Command
{
	protected:
		Server *_server;
		bool _need_auth;
		bool _need_registration;
	public:
		Command();
		virtual ~Command(void);
		virtual void execute(User *user, Message *message);
		bool getNeedAuth(void) const;
		bool getNeedRegistration(void) const;
		void welcome(User *user);
		int reply(std::string response, int fd);
};


class CmdPass : public Command
{
	public:
		CmdPass(Server *server);
		~CmdPass(void);
		void execute(User *user, Message *message);
};

class CmdNick : public Command
{
	public:
		CmdNick(Server *server);
		~CmdNick(void);
		void execute(User *user, Message *message);
};

class CmdUser : public Command
{
	public:
		CmdUser(Server *server);
		~CmdUser(void);
		void execute(User *user, Message *message);
};

class CmdQuit : public Command
{
	public:
		CmdQuit(Server *server);
		~CmdQuit(void);
		void execute(User *user, Message *message);
};

class CmdUnknown : public Command
{
	public:
		CmdUnknown(Server *server);
		~CmdUnknown(void);
		void execute(User *user, Message *message);
};

class CmdMotd : public Command
{
	public:
		CmdMotd(Server *server);
		~CmdMotd(void);
		void execute(User *user, Message *message);
};

class CmdPing : public Command
{
	public:
		CmdPing(Server *server);
		~CmdPing(void);
		void execute(User *user, Message *message);
};

class CmdJoin : public Command
{
	public:
		CmdJoin(Server *server);
		~CmdJoin(void);
		void execute(User *user, Message *message);
};

#endif