/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/23 13:46:54 by cbernot          ###   ########.fr       */
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
	int endpos = str.find(" ");
	std::string s = str.substr(0, endpos);
	for (int i = 0; i < s.size(); i++)
	{
		if (!isalpha(s[i]))
			throw BadCommandException();
	}
	_command = s;
	std::string rawCopy = str;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

void delete_space_element(std::vector<std::string> & tab)
{
	std::vector<std::string>::iterator it_beg = tab.begin();
	std::vector<std::string>::iterator it_end = tab.end();
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
	for (size_t i = 0 ; i < tab.size() ; i++)
	{
		if (tab[i][0] == ':')
		{
			int index = i;
			tab[i].erase(0, 1);
			for (size_t j = i + 1; j < tab.size() ; j++)
			{
				tab[i].append(" " + tab[j]);
				index = j;
			}
			for (size_t j = i + 1; j < tab.size() ; j++)
			{
				tab.erase(tab.begin() + j);
				index = j;
			}
			// tab.erase(tab.end() - 1);
			break;
		}
	}
	while (have_void_element(tab))
	{
		for (size_t i = 0; i < tab.size() ; i++)
		{
			if ((tab[i]).size() == 0)
			{
				tab.erase(tab.begin() + i);
				break;
			}
		}
	}
	_parameters = tab;
}

void Message::processMessage(void)
{
	if (this->_command == "PASS")
	{
		CmdPass cmdPass(_server);
		cmdPass.execute(_author, this);
	}
	else if (this->_command == "USER")
	{
		CmdUser cmdUser(_server);
		cmdUser.execute(_author, this);
	}
	else if (this->_command == "NICK")
	{
		CmdNick cmdNick(_server);
		cmdNick.execute(_author, this);
	}
	else
	{
		if (this->_command != "CAP")
		{
			CmdUnknown cmdUnknown(_server);
			cmdUnknown.execute(_author, this);
		}
	}
}

Message::Message(Server *server, User *user, std::string const &raw)
{
	_server = server;
	_raw = raw;
	// TODO Check raw size. If > 512, throw exception and send ERR_INPUTTOOLONG (417) to client
	std::cout << "[MESSAGE] new message with raw " << _raw << std::endl;
	try
	{
		_raw = getTags(_raw);
		_raw = getSource(_raw);
		_raw = getCommand(_raw);
		getParameters(_raw);
		_author = user;
		std::cout << *this << std::endl;
		processMessage();

		// _response = "Hello";
		// _server->sendData(_response, user.getFD());

		// TODO processmsg();
		// check ta command et l'appeler
		// if (_response)
			// setPollout(author, &dest[0]);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
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
