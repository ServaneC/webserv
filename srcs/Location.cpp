/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:48:09 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/26 14:41:42 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Location.hpp"

Location::Location() : _max_body_size(1000000) {}

Location::~Location() 
{
}

Location::Location(const std::string path, const std::string location_conf, 
    const Location &general) : _path(path), _location_conf(location_conf), 
    _autoindex(false), _max_body_size(1000000)
{
    this->_accepted_methods = parsingAcceptedMethods(location_conf);
    std::cout << "LOCATION -> " << _path << " : ";
    for (std::vector<int>::iterator it = _accepted_methods.begin(); it != _accepted_methods.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;

    if (location_conf.find("root") != std::string::npos)
        this->_root_in_conf = true;
    this->_root = parsingRoot(location_conf, general);
    std::cout << "Local root for <" << path << "> is -> |" << _root << "|" << std::endl;


    this->_indexes = parsingIndexes(location_conf, general);
    std::cout << "Local indexes = ";
        for (std::list<std::string>::iterator it = _indexes.begin(); it != _indexes.end(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
    
    this->_autoindex = parsingAutoIndex(location_conf, general);
    std::cout << "Local autoindex -> " << _autoindex << std::endl;

    this->_cgi_path = parsingCGIconf(location_conf, general);
    std::cout << "Local cgi path -> " << _cgi_path << std::endl;

    this->_max_body_size = parsingBodySize(location_conf, general);
    std::cout << "Local max body size -> " << _max_body_size << std::endl;

    this->_error_page = parsingErrorPage(*this, general);
    std::cout << "Local error_page -> " << _error_page << std::endl;
}

Location::Location(const Location &ref) : _path(ref._path), 
    _location_conf(ref._location_conf), _root(ref._root), 
    _accepted_methods(ref._accepted_methods), _indexes(ref._indexes), 
    _autoindex(ref._autoindex), _cgi_path(ref._cgi_path),
    _max_body_size(ref._max_body_size), _error_page(ref._error_page),
    _root_in_conf(ref._root_in_conf)
{
    std::cout << "copy constructor called" << std::endl; 
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
    // std::cout << "CODE = " << code << std::endl;
    for (std::vector<int>::const_iterator it = this->_accepted_methods.begin(); it != this->_accepted_methods.end(); it++)
    {
        // std::cout << "accepted => " << code << std::endl;
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

bool            Location::getRootInConf() const
{
    return this->_root_in_conf;
}

// void Location::addErrorPagePrefix(std::string prefix)
// {
//     std::cout << "prefixe = " << prefix << std::endl;
//     std::cout << "error avant = " << _error_page << std::endl;
//     this->_error_page.insert(0, prefix);
//     std::cout << "error apres = " << _error_page << std::endl;
// }