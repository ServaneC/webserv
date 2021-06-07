/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:29:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/07 18:30:33 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

size_t findClosingBracket(std::string str, size_t begin)
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