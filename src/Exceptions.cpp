/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:18:12 by cbernot           #+#    #+#             */
/*   Updated: 2024/02/08 11:47:24 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Exceptions.hpp"

const char *BadPortException::what() const throw()
{
	return "\033[0;31mBad port\033[0m";
}

const char *SocketFailedException::what() const throw()
{
	return "\033[0;31mSocket creation failed\033[0m";
}

const char *SetsockoptFailedException::what() const throw()
{
	return "\033[0;31mSetsockopt failed\033[0m";
}

const char *BindFailedException::what() const throw()
{
	return "\033[0;31mBinding port failed\033[0m";
}

const char *ListenFailedException::what() const throw()
{
	return "\033[0;31mListen failed\033[0m";
}

const char *AcceptFailedException::what() const throw()
{
	return "\033[0;31mAccept failed\033[0m";
}

const char *PollFailedException::what() const throw()
{
	return "\033[0;31mPoll failed\033[0m";
}

const char *RecvFailedException::what() const throw()
{
	return "\033[0;31mLoop detected while reading, potential SIGKILL from client\033[0m";
}

const char *FatalException::what() const throw()
{
	return "\033[0;31mFatal error : To Define\033[0m";
}
