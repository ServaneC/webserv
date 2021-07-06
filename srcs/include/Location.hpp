/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/06 13:44:07 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../../webserv.hpp"

class Location
{
    private :

        Server                  *_server;
        std::string             _path;
        std::string             _location_conf;
        std::string             _root;
        std::vector<int>        _accepted_methods;
        std::list<std::string>  _indexes;
        bool                    _autoindex;

        Location();

    public :

        Location(Server *server, std::string path, std::string location_conf);
        ~Location();
        std::string             getPath() const;
        std::string             getRoot() const;
        std::vector<int>        getAcceptedMethods() const;
        bool                    isAcceptedMethod(int code) const;
        bool                    getAutoIndex() const;
        std::list<std::string>  getIndexes() const;
};

#endif