/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 12:10:03 by schene            #+#    #+#             */
/*   Updated: 2021/07/19 20:35:05 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Utils.hpp"

std::string     ft_itoa_cpp(int n)
{
	std::ostringstream convert;
	convert << n;
	return (convert.str());
}

int             hexa_to_int(std::string hexa)
{
    int x;
    std::stringstream ss;
    ss << std::hex << hexa;
    ss >> x;
    return x;
}

int             ft_gnl(unsigned char *buf, int buf_start)
{
    int		next_n = 0;
	while (1)
	{
		if (std::memchr(buf + buf_start + next_n, '\n', 1))
			return (next_n);
		next_n++;
	}
}

size_t ft_atoul(std::string ulong)
{
    size_t x;
    std::stringstream ss;
    ss << std::dec << ulong;
    ss >> x;
    return (x);
}
