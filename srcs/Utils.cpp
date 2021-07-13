/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 12:10:03 by schene            #+#    #+#             */
/*   Updated: 2021/07/13 21:33:33 by lemarabe         ###   ########.fr       */
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
    // unsigned int x;
    int x;
    std::stringstream ss;
    ss << std::hex << hexa;
    ss >> x;
    return x;
}

size_t ft_atoul(std::string ulong)
{
    size_t x;
    std::stringstream ss;
    ss << std::dec << ulong;
    ss >> x;
    return (x);
}
