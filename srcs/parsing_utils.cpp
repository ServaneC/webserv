/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:29:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/09 03:28:45 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

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