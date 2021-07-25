/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 11:42:28 by schene            #+#    #+#             */
/*   Updated: 2021/07/25 16:27:58 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Autoindex.hpp"
    
Autoindex::Autoindex(std::string dir_name, std::string dir_path, const std::list<std::string> indexes) :
    _dir_name(dir_name), _dir_path(dir_path), _index_file(false), _is_dir(true), _exist(true)
{
    struct stat info;

    if (lstat(dir_path.c_str(), &info) < 0)
    {
        this->_exist = false;
        return ;
    }
    if (!S_ISDIR(info.st_mode))
    {
        this->_is_dir = false;
        return ; 
    }
    if ((*(this->_dir_name.end()-1)) == '/')
        this->_dir_name.erase(this->_dir_name.end()-1);
    this->_listing.clear();
    struct dirent *content;
    DIR     *dir = opendir(dir_path.c_str());
    
    if (dir)
    {
        while ((content = readdir(dir))) 
        {
            std::string name = std::string(content->d_name);
            if (content->d_type == DT_DIR)
                name += '/';
            this->_listing.push_back(name);
            for (std::list<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); it++)
            {
                if (!name.compare(*it))
                    _index_file = true;
            }
        }
        closedir(dir);
    }
}

Autoindex::~Autoindex()
{
    this->_listing.clear();
}

std::string Autoindex::autoindex_generator()
{
    std::string index;
    
    index = "<!DOCTYPE HTML>\n<html><body>\n<h1>Index of " + this->_dir_name + "</h1>\n<hr>\n";
    for (std::list<std::string>::iterator it = this->_listing.begin(); it != this->_listing.end(); it++)
        index += "<p><a href = \"" + this->_dir_name + '/' + (*it) + "\">" +  (*it) + "</a></p>\n";
    index += "<hr>\n</body>\n</html>\n";  
    return index;
}

bool        Autoindex::getIndex() const
{
    return this->_index_file;
}

bool        Autoindex::isDir() const
{
    return this->_is_dir;
}

bool        Autoindex::path_exist() const
{
    return this->_exist;
}
