/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/22 10:30:21 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

const char *BadTagException::what() const throw()
{
	return "Bad tag";
}

command stocmd(std::string const &str)
{
	if (str == "CAP")
		return CAP;
	else if (str == "PASS")
		return PASS;
	else if (str == "NICK")
		return NICK;
	else if (str == "USER")
		return USER;
	else if (str == "PING")
		return PING;
	else if (str == "PONG")
		return PONG;
	else if (str == "OPER")
		return OPER;
	else if (str == "QUIT")
		return QUIT;
	else if (str == "KICK")
		return KICK;
	else if (str == "INVITE")
		return INVITE;
	else if (str == "TOPIC")
		return TOPIC;
	else if (str == "MODE")
		return MODE;
	else if (str == "PRIVMSG")
		return PRIVMSG;
	else
		return UNKNOWN;
}

std::string printCmd(command cmd)
{
	switch (cmd)
	{
	case CAP:
		return "CAP"; 
		break;
	case PASS:
		return "PASS"; 
		break;
	case NICK:
		return "NICK"; 
		break;
	case USER:
		return "USER"; 
		break;
	case PING:
		return "PING"; 
		break;
	case PONG:
		return "PONG"; 
		break;
	case OPER:
		return "OPER"; 
		break;
	case QUIT:
		return "QUIT"; 
		break;
	case KICK:
		return "KICK"; 
		break;
	case INVITE:
		return "INVITE"; 
		break;
	case TOPIC:
		return "TOPIC"; 
		break;
	case MODE:
		return "MODE"; 
		break;
	case PRIVMSG:
		return "PRIVMSG";
		break;
	default:
		return "UNKNOWN"; 
		break;
	}
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
	_command = stocmd(s);
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
			tab.erase(tab.end() - 1);
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
	if (_command == PRIVMSG)
	{
		// processPrivmsg();
	}
	else if (_command == PASS)
	{
		std::string serv_pwd = _server->getPassword();
		if (_parameters.size() == 1)
		{
			if (_parameters[0] == serv_pwd)
			{
				_response = "001";
				// _server->addUser();
			}
			else
			{
				// _response = "ERR_PASSWDMISMATCH";
			}
		}
		else
		{
			// _response = "ERR_NEEDMOREPARAMS";
		}

	}
	else if (_command == NICK)
	{
		// processNick();
	}
	else if (_command == USER)
	{
		// processUser();
	}
	else if (_command == PING)
	{
		// processPing();
	}
	else if (_command == PONG)
	{
		// processPong();
	}
	else if (_command == OPER)
	{
		// processOper();
	}
	else if (_command == CAP)
	{
		// processCap();
	}
	else if (_command == KICK)
	{
		// processKick();
	}
	else if (_command == INVITE)
	{
		// processInvite();
	}
	else if (_command == TOPIC)
	{
		// processTopic();
	}
	else if (_command == MODE)
	{
		// processMode();
	}
	else
	{
		std::cout << "Unknown command" << std::endl;
	}
}

Message::Message(std::string const &raw, Server *server)
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
		std::cout << *this << std::endl;
		processMessage();
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

command Message::getCommand(void)
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
	command cmd = rhs.getCommand();
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
	std::cout << "\tCommand: " << printCmd(cmd) << std::endl;
	std::cout << "\tParameters: " << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << "\t\t'" << params[i] << "'" << std::endl;
	return o;
}
