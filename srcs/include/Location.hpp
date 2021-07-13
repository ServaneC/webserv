/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/13 21:38:33 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../../webserv.hpp"

class Location
{
    private :

        // const Server            *_server;
        std::string             _path;
        std::string             _location_conf;
        std::string             _root;
        std::vector<int>        _accepted_methods;
        std::list<std::string>  _indexes;
        bool                    _autoindex;
		std::string 			_cgi_path;  // empty if not in conf
        size_t                  _max_body_size;
        std::string             _error_page;
        Location();

    public :

        Location(const std::string path, const std::string location_conf, const std::string &server_conf, const std::list<Location> &list);
        Location(const Location &ref);
        ~Location();
        const Location &operator=(const Location &ref);

        std::string                     getPath() const;
        std::string                     getRoot() const;
        std::string                     getCGIPath() const;
        std::vector<int>                getAcceptedMethods() const;
        bool                            isAcceptedMethod(int code) const;
        const std::list<std::string>    &getIndexes() const;
		bool                            getAutoIndex() const;
		std::string                     getErrorPage() const;
		size_t                          getMaxBodySize() const;
};

#endif