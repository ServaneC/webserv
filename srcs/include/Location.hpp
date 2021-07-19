/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/19 16:44:00 by lemarabe         ###   ########.fr       */
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
		std::string 			_cgi_path;      // empty if not in conf
        size_t                  _max_body_size; //ulong max if not in conf
        std::string             _error_page;    // empty if not in conf

    public :

        Location();
        Location(const std::string path, const std::string location_conf, const Location &general);
        Location(const Location &ref);
        ~Location();
        const Location &operator=(const Location &ref);

        std::string                     getPath() const;
        std::string                     getConf() const;
        std::string                     getRoot() const;
        std::vector<int>                getAcceptedMethods() const;
        const std::list<std::string>    &getIndexes() const;
		bool                            getAutoIndex() const;
        std::string                     getCGIPath() const;
		size_t                          getMaxBodySize() const;
		std::string                     getErrorPage() const;

        void    setRoot(const std::string &root);
        void    setIndexes(const std::list<std::string> &indexes);
        void    setAutoIndex(bool autoindex);
        void    setCGIpath(const std::string &cgi_path);
        void    setMaxBodySize(const std::string &error_page);
        void    setErrorPage(const std::string &error_page);
        
        bool    isAcceptedMethod(int code) const;
};

#endif