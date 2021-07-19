/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:29:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/19 16:22:02 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

#define PATH_LEN_MAX 1000

size_t findClosingBracket(const std::string &str, size_t begin)
{
	size_t end = begin;
	size_t count = 1;
	
    while (count && end != std::string::npos && str[end])
    {
        end = str.find_first_of("{}", end + 1);
        if (str[end] == '{')
            count++;
        else if (str[end] == '}')
            count--;
    }
	return (end);
}

std::string getScope(const std::string &str, size_t index)
{
	size_t open_bracket = str.find_first_not_of(" \t\n\r\v\f", index);
	size_t close_bracket = findClosingBracket(str, open_bracket);
	
	if (str[open_bracket] == '{' && str[close_bracket] == '}')
		return (str.substr(open_bracket, close_bracket - open_bracket + 1));
	return (std::string());
}

unsigned char    getValueBetweenPoints(const std::string &str, size_t *index)
{
    int result = 0;
    size_t limit = str.find_first_of(".;", *index);

    if (!std::isdigit(str[*index]) || (str[limit] != '.' && str[limit] != ';'))
        throw confIPAddrException();
    result = std::atoi(str.substr(*index, limit - *index).c_str());
    if (result < 0 || result > 255)
        throw confIPAddrException();
    // std::cout << "before cast = " << result << std::endl;
    *index = limit + 1;
    return (static_cast<unsigned char>(result));
}

unsigned int    convertIPAddress(const std::string &conf, size_t index)
{
    IPA_t   addr;
    
    for (int i = 3; i >= 0; i--)
        addr.block[i] = getValueBetweenPoints(conf, &index);
    return (addr.address);
}

bool    isCommentLine(const char *line)
{
    std::string str(line);
    
    size_t firstChar = str.find_first_not_of(" \t\n\r\v\f");
    if (str[firstChar] == '#')
        return (true);
    return (false);
}

int     setMethodCode(const std::string &method_name)
{
    int code = METHOD_NOT_ALLOWED;
    code = !method_name.compare("GET") ? METHOD_GET : code;
    code = !method_name.compare("POST") ? METHOD_POST : code;
    code = !method_name.compare("DELETE") ? METHOD_DELETE : code;
    return (code);
}

int     parseMethod(const std::string &str, size_t *index)
{
    size_t method_index = str.find_first_not_of(" \t\n\r\v\f", *index);
    size_t method_length = str.find_first_of(" \t\n\r\v\f", method_index) - method_index;

    if (str[method_index + method_length - 1] == ';')
        method_length--;
    *index = method_index + method_length;
    if (!str.compare(method_index, method_length, "GET"))
        return (METHOD_GET);
    if (!str.compare(method_index, method_length, "POST"))
        return (METHOD_POST);
    if (!str.compare(method_index, method_length, "DELETE"))
        return (METHOD_DELETE);
    return (METHOD_NOT_ALLOWED);
}

std::string trimLocations(std::string conf)
{
    size_t loc_index = 0;
    while ((loc_index = conf.find("location", loc_index + 1)) < std::string::npos)
    {
        size_t scope_begin = conf.find("{", loc_index);
        conf.erase(scope_begin, findClosingBracket(conf, scope_begin) - scope_begin);
    }
    return (conf);
}

std::string getCurrentDirectory()
{
    char buf[PATH_LEN_MAX];
    std::string root(getcwd(buf, PATH_LEN_MAX));
    return (root);
}