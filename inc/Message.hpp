/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:14:43 by cbernot           #+#    #+#             */
/*   Updated: 2024/01/24 21:13:17 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ircserv.hpp"

// https://modern.ircdocs.horse/#message-format
class Message
{
private:
	std::string _raw;
	std::vector<std::string> _tags;
	std::string _source;
	std::string _command;
	std::vector<std::string> _parameters;
	std::string _response;
	void	parser(void);

public:
	Message(void);
	Message(std::string const &raw);
	~Message(void);

	std::string const &getRaw(void) const;
	std::vector<std::string> getTags(void);
	std::string const &getSource(void) const;
	std::string const &getCommand(void) const;
	std::vector<std::string> getParameters(void);
	std::string const &getResponse(void) const;

	void setTags(std::string tags);
};

#endif

// /join LoremipsumdolorsitametconsectetueradipiscingelitAeneancommodoligulaegetdolorAeneanmassaCumsociisnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmusDonecquamfelisultriciesnecpellentesqueeupretiumquissemNullaconsequatmassaquisenimDonecpedejustofringillavelaliquetnecvulputateegetarcuInenimjustorhoncusutimperdietavenenatisvitaejustoNullamdictumfeliseupedemollispretiumIntegertinciduntCrasdapibusVivamusellldldldlldldlldlldldlldlldldldlldLoremipsumdolorsitametconsectetueradipiscingelitAeneancommodoligulaegetdolorAeneanmassaCumsociisnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmusDonecquamfelisultriciesnecpellentesqueeupretiumquissemNullaconsequatmassaquisenimDonecpedejustofringillavelaliquetnecvulputateegetarcuInenimjustorhoncusutimperdietavenenatisvitaejustoNullamdictumfeliseupedemollispretiumIntegertinciduntCrasdapibusVivamusellldldldlldldlldlldldlldlldldldllddxxxxxxxxxxxxxxxxxxxxxxx