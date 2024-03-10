/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:36:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 00:50:21 by cbernot          ###   ########.fr       */
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
		virtual void execute(Message *message);
		bool getNeedAuth(void) const;
		bool getNeedRegistration(void) const;
		void welcome(Message *message);
		int reply(std::string sender, std::string response, int fd);
};


class CmdPass : public Command
{
	public:
		CmdPass(Server *server);
		~CmdPass(void);
		void execute(Message *message);
};

class CmdNick : public Command
{
	public:
		CmdNick(Server *server);
		~CmdNick(void);
		void execute(Message *message);
};

class CmdUser : public Command
{
	public:
		CmdUser(Server *server);
		~CmdUser(void);
		void execute(Message *message);
};

class CmdQuit : public Command
{
	public:
		CmdQuit(Server *server);
		~CmdQuit(void);
		void execute(Message *message);
};

class CmdUnknown : public Command
{
	public:
		CmdUnknown(Server *server);
		~CmdUnknown(void);
		void execute(Message *message);
};

class CmdMotd : public Command
{
	public:
		CmdMotd(Server *server);
		~CmdMotd(void);
		void execute(Message *message);
};

class CmdPing : public Command
{
	public:
		CmdPing(Server *server);
		~CmdPing(void);
		void execute(Message *message);
};

class CmdJoin : public Command
{
	private:
		bool isChanNameValid (std::string const & name);
		void sendJoinMsg(User *user, Channel *chan);
	public:
		CmdJoin(Server *server);
		~CmdJoin(void);
		void execute(Message *message);
};

class CmdPart : public Command
{
	public:
		CmdPart(Server *server);
		~CmdPart(void);
		void execute(Message *message);
};

class CmdMode : public Command
{
	private:
		void handleModeString(std::vector<std::string> args, Channel *chan, User *user);
	public:
		CmdMode(Server *server);
		~CmdMode(void);
		void execute(Message *message);
};

class CmdTopic : public Command
{
	public:
		CmdTopic(Server *server);
		~CmdTopic(void);
		void execute(Message *message);
};

class CmdKick : public Command
{
	public:
		CmdKick(Server *server);
		~CmdKick(void);
		void execute(Message *message);
};

class CmdInvite : public Command
{
	public:
		CmdInvite(Server *server);
		~CmdInvite(void);
		void execute(Message *message);
};

#endif