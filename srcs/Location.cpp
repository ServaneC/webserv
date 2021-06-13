/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:48:09 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/13 12:49:46 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(std::string path, std::string location_conf) : _path(path), _location_conf(location_conf)
{
    this->_accepted_methods = parseAcceptedMethods(location_conf);
}

bool Location::isAcceptedMethod(int code)
{
    for (std::vector<int>::iterator it = this->_accepted_methods.begin(); it != this->_accepted_methods.end(); it++)
    {
        if (*it == code)
            return true;
    }
    return false;
}