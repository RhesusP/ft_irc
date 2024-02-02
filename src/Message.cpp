/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:17:28 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/02 15:27:26 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"

const char *BadTagException::what() const throw()
{
	return "Bad tag";
}

command stocmd(std::string const &str)
{
	std::cout << "command to assign: " << str << std::endl;
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
		if (tag_tab.size() != 2)
			throw BadTagException();
		std::vector<std::string>::iterator ita = tag_tab.begin();
		std::string key = *ita;
		++ita;
		std::string value = *ita;
		_tags.insert(std::pair<std::string, std::string>(key, value));
		tag_tab.clear();
	}
	std::string rawCopy = raw;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

std::string Message::getSource(std::string const &raw)
{
	int i = 0;
	while (raw[i] == ' ')
		i++;
	if (raw[i] != ':')
		return raw;
	int endpos = raw.find(" ");
	_source = raw.substr(1, endpos - 1);
	std::string rawCopy = raw;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

std::string Message::getCommand(std::string const &raw)
{
	int endpos = raw.find(" ");
	std::string str = raw.substr(0, endpos);
	if (str.size() == 3 && isdigit(str[0]))	//TODO /!\ To handle
	{
		for (int i = 0; i < str.size(); i++)
		{
			if (!isdigit(str[i]))
				throw BadCommandException();
		}
	}
	else
	{
		for (int i = 0; i < str.size(); i++)
		{
			if (!isalpha(str[i]))
				throw BadCommandException();
		}
		_command = stocmd(str);
	}
	std::string rawCopy = raw;
	rawCopy.erase(0, endpos + 1);
	return rawCopy;
}

void getParameters(std::string const &raw)
{
	// TODO need to split raw by space delimiter and remove extra spaces
	// TODO handle trailing ? (https://modern.ircdocs.horse/#parameters)
}

Message::Message(std::string const &raw)
{
	_raw = raw;
	// TODO Check raw size. If > 512, throw exception and send ERR_INPUTTOOLONG (417) to client
	std::cout << "[MESSAGE] new message with raw " << _raw << std::endl;
	try
	{
		_raw = getTags(_raw);
		_raw = getSource(_raw);
		_raw = getCommand(_raw);
		// getParameters(_raw);
		std::cout << *this << std::endl;
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
	std::cout << "\tCommand:" << printCmd(cmd) << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << "\t\t" << params[i] << std::endl;
	return o;
}