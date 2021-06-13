/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/11 18:56:40 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../../webserv.hpp"

class Location
{
    private :

        std::string         _path;
        std::string         _location_conf;
        std::vector<int>    _accepted_methods;

        Location();

    public :

        Location(std::string path, std::string location_conf);
        ~Location();
        bool isAcceptedMethod(int code);
          
};

#endif