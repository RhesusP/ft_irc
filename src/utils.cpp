/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:56:55 by svanmeen          #+#    #+#             */
/*   Updated: 2024/02/15 11:01:54 by svanmeen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ircserv.hpp" 

std::string	receve(int fd) { // TODO : Watch out for overlap 512
	std::string	ret;
	char		buf[BUFF_SIZE];
	int 		byteread;

	byteread = recv(fd, buf, BUFF_SIZE - 1, 0);
	if (byteread <= 0)
		throw RecvFailedException();
	buf[byteread] = '\0';
	ret = buf;
	while (byteread == BUFF_SIZE - 1 && ret.size() < 512) {
		if (byteread <= 0)
			throw RecvFailedException();
		byteread = recv(fd, buf, BUFF_SIZE - 1, 0);
		buf[byteread] = '\0';
		ret += buf;
	}
	return (ret);
}

