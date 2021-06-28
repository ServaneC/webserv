/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/28 14:25:15 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../../webserv.hpp"

class Location
{
    private :

        Server              *_server;
        std::string         _path;
        std::string         _location_conf;
        std::string         _root_path;
        std::string         _root;
        std::vector<int>    _accepted_methods;

        Location();

    public :

        Location(Server *server, std::string path, std::string location_conf);
        ~Location();
        std::string         getPath() const;
        std::string         getRoot() const;
        std::string         getRootPath() const;
        std::vector<int>    getAcceptedMethods() const;
        bool                isAcceptedMethod(std::string method);
};

#endif