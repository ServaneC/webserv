/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 12:10:03 by schene            #+#    #+#             */
/*   Updated: 2021/06/15 13:58:38 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Utils.hpp"

std::string     ft_itoa_cpp(int n)
{
	std::ostringstream convert;
	convert << n;
	return (convert.str());
}

int             ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return(i);
}

