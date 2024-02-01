/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:18:12 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/01 15:03:15 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Exceptions.hpp"

const char *BadPortException::what() const throw()
{
	return "Bad port";
}

const char *SocketFailedException::what() const throw()
{
	return "Socket creation failed";
}

const char *SetsockoptFailedException::what() const throw()
{
	return "Setsockopt failed";
}

const char *BindFailedException::what() const throw()
{
	return "Binding port failed";
}

const char *ListenFailedException::what() const throw()
{
	return "Listen failed";
}

const char *AcceptFailedException::what() const throw()
{
	return "Accept failed";
}

const char *BadCommandException::what() const throw()
{
	return "Unknown command";
}

