/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/13 21:41:55 by lemarabe         ###   ########.fr       */
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

size_t              findClosingBracket(const std::string &str, size_t begin);
std::string         getScope(const std::string &str, size_t index);
unsigned char       getValueBetweenPoints(const std::string &str, size_t *index);
bool                isCommentLine(const char *line);
std::string         parsingName(const std::string &conf);
unsigned int        convertIPAddress(const std::string &conf, size_t index);
void                parsingIPAddress(const std::string &conf, unsigned int *ip, int *port);
std::list<Location> parsingLocations(const std::string &conf);
int                 setMethodCode(const std::string &method_name);
std::vector<int>    parseAcceptedMethods(const std::string &location_conf);
int                 parseMethod(const std::string &str, size_t *index);
std::string         parsingRoot(const std::string &loc_conf, const std::string &server_conf, const std::list<Location> &list);//, const std::string &loc_path);
std::string         trimLocations(std::string conf);
std::string         getCurrentDirectory();
// std::string         parsingLocalRoot(std::string server_root, std::string conf);
// std::string         translatePath(Server &server, Request &request, const std::string &target, std::string &object);
const Location      &getRelevantExtension(const std::list<Location> &_routes, const std::string &target);
const Location      &getRelevantLocation(const std::list<Location> &_routes, const std::string &target);
std::list<std::string> parsingIndexes(const std::string &loc_conf, const std::string &server_conf);
bool                    parsingAutoIndex(const std::string &loc_conf, const std::string &server_conf);
std::string             buildPath(Server &server, Request &request, const std::string &target);
std::string             parsingCGIconf(const std::string &loc_conf, const std::string &server_conf);
const std::string       firstValidIndex(const std::list<std::string> &indexes);
const Location          *findRootLocation(const std::list<Location> &list);
size_t                  parsingBodySize(const std::string &loc_conf, const std::string &server_conf);
std::string             parsingErrorPage(const std::string &loc_conf, const std::string &server_conf);
std::string             &absolutePath(const Location &loc, std::string &name);
#endif