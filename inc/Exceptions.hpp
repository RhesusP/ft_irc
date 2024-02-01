/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:14:50 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/01 15:00:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "ircserv.hpp"

class BadPortException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class SocketFailedException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class SetsockoptFailedException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class BindFailedException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class ListenFailedException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class AcceptFailedException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class BadCommandException : public std::exception
{
public:
	virtual const char *what() const throw();
};

#endif