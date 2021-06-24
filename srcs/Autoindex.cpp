/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 11:42:28 by schene            #+#    #+#             */
/*   Updated: 2021/06/24 15:10:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Autoindex.hpp"

Autoindex::Autoindex(std::string dir_name, std::string dir_path) :
    _dir_name(dir_name), _dir_path(dir_path), _index_file(false)
{
    if (dir_name[dir_name.size() - 1] != '/')
        return ;
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
            if (!name.compare("index.html"))
                _index_file = true;
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
    
    index = "<!DOCTYPE HTML>\n";
    index += "<body>\n<h1>Index of " + this->_dir_name + "</h1>\n<hr>\n";
    for (std::list<std::string>::iterator it = this->_listing.begin(); it != this->_listing.end(); it++)
        index += "<p><a href = \"" + (*it) + "\">" +  (*it) + "</a></p>\n";
    index += "<hr>\n</body>\n</html>\n";  
    return index;
}

bool        Autoindex::getIndex() const
{
    return this->_index_file;
}

bool        Autoindex::isDir() const
{
    return (this->_dir_name[this->_dir_name.size() - 1] == '/');
}

