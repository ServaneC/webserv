/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:48:09 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/22 15:39:35 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(Server *server, std::string path, std::string location_conf) : _server(server), _path(path), _location_conf(location_conf)
{
    this->_accepted_methods = parseAcceptedMethods(location_conf);
    // std::cout << "LOCATION -> " << _path << " : ";
    // for (std::vector<int>::iterator it = _accepted_methods.begin(); it != _accepted_methods.end(); it++)
    //     std::cout << *it << " ";
    // std::cout << std::endl;
    this->_root = parsingLocalRoot(_server->getRoot(), location_conf);
    std::cout << "Local root for <" << path << " is -> |" << _root << "|" << std::endl;
}

bool Location::isAcceptedMethod(std::string method)
{
    int code = 0;
    code = !method.compare("GET") ? METHOD_GET : code;
    code = !method.compare("POST") ? METHOD_POST : code;
    code = !method.compare("DELETE") ? METHOD_DELETE : code;
    for (std::vector<int>::iterator it = this->_accepted_methods.begin(); it != this->_accepted_methods.end(); it++)
    {
         if (*it == code)
            return true;
    }
    return false;
}

std::string         Location::getPath() const 
{ 
    return (this->_path);
}
std::string         Location::getRoot() const
{
    return (this->_root);
}
std::vector<int>    Location::getAcceptedMethods() const
{
    return (this->_accepted_methods);
}