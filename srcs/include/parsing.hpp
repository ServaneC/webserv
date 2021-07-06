/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/06 03:00:14 by lemarabe         ###   ########.fr       */
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
std::string         translatePath(Server &server, Request &request, std::map<std::string, std::string> &cgi_env, std::string path_info);
Location            *getRelevantExtension(std::list<Location> &_routes, std::string target);
Location            *getRelevantLocation(std::list<Location> &_routes, std::string target);
std::list<std::string> parsingIndexes(const std::list<std::string> *root_indexes, std::string conf);
bool parsingAutoIndex(Server &server, std::string conf);
// bool isDirectory(const std::string &target);

#endif