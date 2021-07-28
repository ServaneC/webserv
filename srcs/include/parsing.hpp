/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/28 12:13:16 by schene           ###   ########.fr       */
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


// **************** UTILS **************** //

size_t              findClosingBracket(const std::string &str, size_t begin);
std::string         getScope(const std::string &str, size_t index);
unsigned char       getValueBetweenPoints(const std::string &str, size_t *index);
unsigned int        convertIPAddress(const std::string &conf, size_t index);
bool                isCommentLine(const char *line);
int                 setMethodCode(const std::string &method_name);
int                 parseMethod(const std::string &str, size_t *index);
std::string         trimLocations(std::string conf);
std::string         getCurrentDirectory();

// ************ PARSING SERVER ************ //

std::string         parsingName(const std::string &conf);
std::string         parsingRoot(const std::string &loc_conf, const Location &general);
std::string         parsingIPAddress(const std::string &conf, unsigned int *ip, int *port);
void                parsingLocations(std::list<Location*> &routes, const std::string &conf);

// *********** PARSING LOCATIONS *********** //

std::vector<int>        parsingAcceptedMethods(const std::string &location_conf);
std::list<std::string>  parsingIndexes(const std::string &loc_conf, const Location &general);
bool                    parsingAutoIndex(const std::string &loc_conf, const Location &general);
std::string             parsingCGIconf(const std::string &loc_conf, const Location &general);
size_t                  parsingBodySize(const std::string &loc_conf, const Location &general);
std::string             parsingErrorPage(const Location &location, const Location &general);
std::string             parsingRedirection(const std::string &loc_conf, const Location &general);
std::string             parsingUploadPath(const std::string &loc_conf, const Location &general);

// *********** HANDLING LOCATIONS *********** //

const Location          &getRelevantLocation(const std::list<Location*> &_routes, const std::string &target);
std::string             buildPath(Server &server, Request &request, const std::string &target);
const std::string       firstValidIndex(const std::list<std::string> &indexes);
const Location          *findRootLocation(const std::list<Location*> &list);
// void            updateWithRootInfos(std::list<Location> &routes, const Location *root);

#endif