/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:40:42 by schene            #+#    #+#             */
/*   Updated: 2021/07/09 14:35:55 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "../../webserv.hpp"

class Autoindex
{
    private:
        std::string                     _dir_name;
        std::string                     _dir_path;
        std::list<std::string>          _listing;
        bool                            _index_file;
        bool                            _is_dir;
    
    public:
        Autoindex(std::string dir_name, std::string dir_path, const std::list<std::string> indexes);
        ~Autoindex();

        std::string autoindex_generator();
        bool        getIndex() const;
        bool        isDir() const;
};

#endif
