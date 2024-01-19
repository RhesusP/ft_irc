/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:18:12 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/19 15:28:13 by svanmeen         ###   ########.fr       */
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

const char *PollFailedException::what() const throw()
{
	return "Poll failed";
}
