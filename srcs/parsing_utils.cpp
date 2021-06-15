/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:29:12 by lemarabe          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/06/15 15:46:25 by lemarabe         ###   ########.fr       */
=======
/*   Updated: 2021/06/14 00:36:47 by lemarabe         ###   ########.fr       */
>>>>>>> 58064780177b432f033f0372d32d9d50318e41aa
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

size_t findClosingBracket(const std::string str, size_t begin)
{
	size_t end = begin + 1;
	size_t count = 1;
	
    while (count && str[end] && end != std::string::npos)
    {
        end = str.find_first_of("{}", end + 1);
        if (str[end] == '{')
            count++;
        else
            count--;
    }
	return (end);
}

std::string getScope(const std::string str, size_t index)
{
	size_t open_bracket = str.find_first_not_of(" \t\n\r\v\f", index);
	// std::cout << open_bracket << " = " << str[open_bracket] << std::endl;
	size_t close_bracket = findClosingBracket(str, open_bracket);
	// std::cout << close_bracket << " = " << str[close_bracket] << std::endl;
	
	if (str[open_bracket] == '{' && str[close_bracket] == '}')
		return (str.substr(open_bracket, close_bracket - open_bracket + 1));
	return (std::string());
}

unsigned char    getValueBetweenPoints(const std::string str, size_t *index)
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

bool    isCommentLine(const char *line)
{
    std::string str(line);
    
    size_t firstChar = str.find_first_not_of(" \t\n\r\v\f");
    if (str[firstChar] == '#')
        return (true);
    return (false);
}

int     parseMethod(std::string str, size_t *index)
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