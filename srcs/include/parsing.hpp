/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/06/15 15:48:58 by lemarabe         ###   ########.fr       */
=======
/*   Updated: 2021/06/13 23:50:47 by lemarabe         ###   ########.fr       */
>>>>>>> 58064780177b432f033f0372d32d9d50318e41aa
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "../../webserv.hpp"

#define METHOD_NOT_ALLOWED 0
#define METHOD_GET 1
#define METHOD_POST 2
#define METHOD_DELETE 3

typedef union u_IPAddress
{
    unsigned char   block[4];
    unsigned int    address;
}                       IPA_t;

size_t              findClosingBracket(const std::string str, size_t begin);
std::string         getScope(const std::string str, size_t index);
unsigned char       getValueBetweenPoints(const std::string str, size_t *index);
bool                isCommentLine(const char *line);
std::string         parsingName(std::string conf);
unsigned int        convertIPAddress(std::string conf, size_t index);
void                parsingIPAddress(std::string conf, unsigned int *ip, int *port);
std::list<Location> parsingLocations(std::string conf);
int                 setMethodCode(std::string method_name);
std::vector<int>    parseAcceptedMethods(std::string location_conf);
int                 parseMethod(std::string str, size_t *index);

#endif