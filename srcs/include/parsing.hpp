/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/21 03:13:49 by lemarabe         ###   ########.fr       */
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
std::list<Location> parsingLocations(Server &server, std::string conf);
int                 setMethodCode(std::string method_name);
std::vector<int>    parseAcceptedMethods(std::string location_conf);
int                 parseMethod(std::string str, size_t *index);
std::string         parsingRoot(std::string conf);
std::string         trimLocations(std::string conf);
std::string         getCurrentDirectory();
std::string         parsingLocalRoot(std::string server_root, std::string conf);


#endif