/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/31 23:27:56 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"

class BadTagException : public std::exception
{
public:
	virtual const char *what() const throw();
};

class Message
{
private:
	std::string _raw;
	std::map<std::string, std::string>	_tags;
	std::string _response;
	void getTags(std::string const & str);

public:
	Message(void);
	Message(std::string const &raw);
	~Message(void);
	// std::string const &getRaw(void) const;
	// std::string const &getContent(void) const;
	// std::string const &getResponse(void) const;
};

#endif

// /join LoremipsumdolorsitametconsectetueradipiscingelitAeneancommodoligulaegetdolorAeneanmassaCumsociisnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmusDonecquamfelisultriciesnecpellentesqueeupretiumquissemNullaconsequatmassaquisenimDonecpedejustofringillavelaliquetnecvulputateegetarcuInenimjustorhoncusutimperdietavenenatisvitaejustoNullamdictumfeliseupedemollispretiumIntegertinciduntCrasdapibusVivamusellldldldlldldlldlldldlldlldldldlldLoremipsumdolorsitametconsectetueradipiscingelitAeneancommodoligulaegetdolorAeneanmassaCumsociisnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmusDonecquamfelisultriciesnecpellentesqueeupretiumquissemNullaconsequatmassaquisenimDonecpedejustofringillavelaliquetnecvulputateegetarcuInenimjustorhoncusutimperdietavenenatisvitaejustoNullamdictumfeliseupedemollispretiumIntegertinciduntCrasdapibusVivamusellldldldlldldlldlldldlldlldldldllddxxxxxxxxxxxxxxxxxxxxxxx


//  @url=;netsplit=tur,ty