/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:48:09 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/11 19:21:13 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(std::string path, std::string location_conf) : _path(_path), _location_conf(location_conf)
{
    _accepted_methods = parseAcceptedMethods(location_conf);
}

bool isAcceptedMethod(int code)
{
    for (std::vector<int>::iterator it = _accepted_methods.begin(); it != _accepted_methods.end(); it++)
    {
        if (*it == code)
            return true;
    }
    return false;
}