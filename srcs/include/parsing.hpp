/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/10 16:22:02 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "../../webserv.hpp"

typedef union u_IPAddress
{
    unsigned char   block[4];
    unsigned int    address;
}                       IPA_t;

size_t          findClosingBracket(const std::string str, size_t begin);
std::string     getScope(const std::string str, size_t index);
unsigned char   getValueBetweenPoints(const std::string str, size_t *index);
bool            isCommentLine(const char *line);
std::string     parsingName(std::string conf);
unsigned int    convertIPAddress(std::string conf, size_t index);
void            parsingIPAddress(std::string conf, unsigned int *ip, int *port);
std::map<std::string, std::string>  parsingLocations(std::string conf);

#endif