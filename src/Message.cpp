/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/03/10 00:47:20 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

const char *BadTagException::what() const throw()
{
	return "Bad tag";
}

Message::Message(void)
{
}

std::string Message::getTags(std::string const &raw)
{
	if (raw[0] != '@')
		return raw;
	int endpos = raw.find(" ");
	std::string str = raw.substr(1, endpos - 1);
	std::vector<std::string> tags_tab = ::split(str, ";");
	std::vector<std::string> tag_tab;
	for (std::vector<std::string>::iterator it = tags_tab.begin(); it != tags_tab.end(); it++)
	{
		tag_tab = ::split(*it, "=");
		if (tag_tab.size() > 2)
			throw BadTagException();
		std::vector<std::string>::iterator ita = tag_tab.begin();
		std::string key = *ita;
		++ita;
		std::string value;
		if (ita == tag_tab.end())
			value = "";
		else
			value = *ita;
		_tags.insert(std::pair<std::string, std::string>(key, value));
		tag_tab.clear();
	}
	std::string rawCopy = raw;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

std::string Message::getSource(std::string const &raw)
{
	std::string str = trim(raw, " ");
	if (str[0] != ':')
		return str;
	int endpos = str.find(" ");
	_source = str.substr(1, endpos - 1);
	std::string rawCopy = str;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

std::string Message::getCommand(std::string const &raw)
{
	std::string str = trim(raw, " ");
	std::string s;
	size_t endpos = str.find(" ");
	if (endpos != std::string::npos)
		s = str.substr(0, endpos);
	else
		s = str;
	_command = s;
	std::string rawCopy = str;
	if (endpos != std::string::npos)
		rawCopy.erase(0, endpos + 1);
	else
		rawCopy.clear();
	return rawCopy;
}

void delete_space_element(std::vector<std::string> & tab)
{
	std::vector<std::string>::iterator it_beg = tab.begin();
	std::vector<std::string>::iterator it_temp;

	while (it_beg != tab.end())
	{
		if ((*it_beg).size() == 0)
		{
			it_temp = it_beg + 1;
			tab.erase(it_beg);
			it_beg = it_temp;
		}
		else
			++it_beg;
	}
}

bool have_void_element(std::vector<std::string> & tab)
{
	for (size_t i = 0; i < tab.size() ; i++)
	{
		if ((tab[i]).size() == 0)
			return true;
	}
	return false;
}

void Message::getParameters(std::string const &raw)
{
	std::vector<std::string> tab = split(raw, " ");
	std::vector<std::string> new_tab;
	for (size_t i = 0 ; i < tab.size() ; i++)
	{
		if (tab[i][0] == ':')
		{
			std::string remain = tab[i];
			for (size_t j = i + 1; j < tab.size() ; j++)
			{
				remain.append(" " + tab[j]);
			}
			new_tab.push_back(remain);
			break;
		}
		else
		{
			new_tab.push_back(tab[i]);
		}
	}
	for (size_t i = 0; i < new_tab.size() ; i++)
	{
		if (new_tab[i][0] == ':')
		{
			new_tab[i].erase(0, 1);
		}
	}
	_parameters = new_tab;
}

void Message::processMessage(void)
{
	int nb_cmds = 12;
	std::string cmds_name [nb_cmds] = {"MOTD", "NICK", "PASS", "PING", "QUIT", "UNKNOWN", "USER", "JOIN", "PART", "MODE", "TOPIC", "KICK"};
	CmdMotd cmdMotd(_server);
	CmdNick cmdNick(_server);
	CmdPass cmdPass(_server);
	CmdPing cmdPing(_server);
	CmdQuit cmdQuit(_server);
	CmdUnknown mdUnknown(_server);
	CmdUser cmdUser(_server);
	CmdJoin CmdJoin(_server);
	CmdPart CmdPart(_server);
	CmdMode CmdMode(_server);
	CmdTopic CmdTopic(_server);
	CmdKick CmdKick(_server);

	Command* cmds[nb_cmds] = {
		&cmdMotd,
		&cmdNick,
		&cmdPass,
		&cmdPing,
		&cmdQuit,
		&mdUnknown,
		&cmdUser,
		&CmdJoin,
		&CmdPart,
		&CmdMode,
		&CmdTopic,
		&CmdKick
	};
	for (int i = 0; i < nb_cmds; i++)
	{
		if (this->_command == cmds_name[i])
		{
			if ((cmds[i]->getNeedAuth() && !_author->getIsAuth()) || (cmds[i]->getNeedRegistration() && !_author->isRegistered()))
			{
				_server->sendData(_server->getName(), ERR_NOTREGISTERED(_author->getNickname()), _author->getFD());
				return;
			}
			// if (this->_command == "PASS" || this->_command == "NICK" || this->_command == "USER")
				// cmds[i]->execute(this);
			cmds[i]->execute(this);
			return;
		}
	}
	if (this->_command != "CAP")
	{
		CmdUnknown cmdUnknown(_server);
		cmdUnknown.execute(this);
	}
}

Message::Message(Server *server, User *user, std::string const &raw)
{
	_server = server;
	_raw = raw;
	// TODO Check raw size. If > 512, throw exception and send ERR_INPUTTOOLONG (417) to client
	_raw = getTags(_raw);
	_raw = getSource(_raw);
	_raw = getCommand(_raw);
	getParameters(_raw);
	_author = user;
	std::cout << *this;
	processMessage();
}

Message::~Message(void) {}

std::map<std::string, std::string> &Message::getTags(void)
{
	return _tags;
}

std::string const &Message::getSource(void)
{
	return _source;
}

std::string const & Message::getCommand(void) const
{
	return _command;
}

std::vector<std::string> Message::getParameters(void)
{
	return _parameters;
}

User* Message::getAuthor(void) const
{
	return _author;
}

std::ostream &operator<<(std::ostream &o, Message &rhs)
{
	std::map<std::string, std::string> tags = rhs.getTags();
	std::string source = rhs.getSource();
	std::vector<std::string> params = rhs.getParameters();

	std::cout << "============ NEW MESSAGE ============" << std::endl;
	std::cout << "\tTags: " << std::endl;
	std::map<std::string, std::string>::iterator it_beg_tags = tags.begin();
	std::map<std::string, std::string>::iterator it_end_tags = tags.end();
	while (it_beg_tags != it_end_tags)
	{
		std::cout << "\t\t" << it_beg_tags->first << "=" << it_beg_tags->second << std::endl;
		++it_beg_tags;
	}
	std::cout << "\tSource: " << source << std::endl;
	std::cout << "\tCommand: " << rhs.getCommand() << std::endl;
	std::cout << "\tParameters: " << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << "\t\t'" << params[i] << "'" << std::endl;
	return o;
}
