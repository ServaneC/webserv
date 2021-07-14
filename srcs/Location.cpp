/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:48:09 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/14 20:25:54 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(const std::string path, const std::string location_conf,
    const std::string &server_conf, const std::list<Location> &list) : _path(path), _location_conf(location_conf),
    _autoindex(false)
{
    this->_accepted_methods = parseAcceptedMethods(location_conf);
    std::cout << "LOCATION -> " << _path << " : ";
    // for (std::vector<int>::iterator it = _accepted_methods.begin(); it != _accepted_methods.end(); it++)
    //     std::cout << *it << " ";
    // std::cout << std::endl;

    this->_root = parsingRoot(location_conf, server_conf, list);
    // std::cout << "Local root for <" << path << "> is -> |" << _root << "|" << std::endl;

    this->_indexes = parsingIndexes(location_conf, server_conf);
    // std::cout << "Local indexes = ";
    //     for (std::list<std::string>::iterator it = _indexes.begin(); it != _indexes.end(); it++)
    //         std::cout << *it << " ";
    //     std::cout << std::endl;
    
    this->_autoindex = parsingAutoIndex(location_conf, server_conf);
    // std::cout << "Local autoindex -> " << _autoindex << std::endl;

    this->_cgi_path = parsingCGIconf(location_conf, server_conf);
    // std::cout << "Local cgi path -> " << _cgi_path << std::endl;

    this->_max_body_size = parsingBodySize(location_conf, server_conf);
    // std::cout << "Local max body size -> " << _max_body_size << std::endl;

    Location &root = *findRootLocation(list);
    this->_error_page = parsingErrorPage(*this, root);
    std::cout << "Local error_page -> " << _error_page << std::endl;
}

Location::Location(const Location &ref) : _path(ref._path), 
    _location_conf(ref._location_conf), _root(ref._root), 
    _accepted_methods(ref._accepted_methods), _indexes(ref._indexes), 
    _autoindex(ref._autoindex), _cgi_path(ref._cgi_path)
{
    // std::cout << "copy constructor called" << std::endl; 
}

const Location &Location::operator=(const Location &ref)
{
    _path = ref._path; 
    _location_conf = ref._location_conf;
    _root = ref._root;
    _accepted_methods = ref._accepted_methods;
    _indexes = ref._indexes;
    _autoindex = ref._autoindex;
    _cgi_path = ref._cgi_path;
    return (*this);
}

bool Location::isAcceptedMethod(int code) const
{
    for (std::vector<int>::const_iterator it = this->_accepted_methods.begin(); it != this->_accepted_methods.end(); it++)
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

const std::list<std::string>	&Location::getIndexes() const
{
    return (this->_indexes);
}

bool					Location::getAutoIndex() const
{
    return (this->_autoindex);
}

std::string         Location::getCGIPath() const
{
    return (this->_cgi_path);
}

std::string     Location::getErrorPage() const {
    return (this->_error_page);
}
size_t          Location::getMaxBodySize() const {
    return (this->_max_body_size);
}
std::string     Location::getConf() const {
    return (this->_location_conf);
}