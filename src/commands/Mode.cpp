/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:31:06 by svanmeen          #+#    #+#             */
/*   Updated: 2024/03/05 14:54:39 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/Commands.hpp"

CmdMode::inputOk(std::string modes) {
	int count = 0;
	int br = 0;

	if (modes[0] != "+" || modes[0] != "-")
		return false;
	if ((int i = modes.find("+")) == std::string::npos)
		i = -1;
	if ((int j = modes.find("-")) == std::string::npos)
		j = -1;
	if ( i == j)
		return false;
	for (int k = 1; k < (j < i) ? i : j; k++)
	{
		int l = k;
		while (modes[l] != ((j < i) ? i : j))
		{
			if (k == l)
				return false;
			l++;
		}
	}
	for (int k = (j < i ? i : j); k < modes.size(); k++)
	{
		int l = k;
		while (modes[l] != modes.size())
		{
			if (k == l)
				return false;
			l++;
		}
	}
}

CmdMode::CmdMode(Server *server)
{
	_server = server;
	_need_auth = false;
	_need_registration = false;
}

CmdMode::~CmdMode(void) {}

void	CmdMode::execute(Message *message) 
{
	User *usr = message->getAuthor();
	std::vector<std::string> args = message->getParameters();
	std::string name = args[0];
	name = name[1];
	std::string mmodes = args[1];

	size_t index;

	for (size_t i = 0; i < usr->getChannels().size(); i++)
	{
		if (name == usr->getChannels().at(i)->getName()) {
			index = i;
		}
	}
	Channel *chan = usr->getChannels().at(index);
	if (!chan->isOperator(usr))
		return ; //TODO RPL_NOTOP
	int modes = chan->getModes();
	int cor = 0;

	if (inputOk(mmodes))
		return;
	if (mmodes[0] == '+') {
		int bl = mmodes.find("-");
		for (int i = 1; i < bl; i++)
		{
			switch (mmodes[i])
			{
				case 'i':
					cor += 1 << 4;
					break;
				case 't':
					cor += 1 << 3;
					break;
				case 'k':
					cor += 1 << 2;
					break;
				case 'o':
					cor += 1 << 1;
					break;
				case 'l':
					cor += 1;
					break;
			}
		}
	}
	else
	
}
