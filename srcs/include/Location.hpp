/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:43:38 by lemarabe          #+#    #+#             */
/*   Updated: 2021/08/02 18:07:10 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../../webserv.hpp"

class Location
{
    private :

        // const Server            *_server;
        std::string                 _path;
        std::string                 _location_conf;
        std::string                 _root;
        std::vector<int>            _accepted_methods;
        std::list<std::string>      _indexes;
        bool                        _autoindex;
		std::string 			    _cgi_path;      // empty if not in conf
        size_t                      _max_body_size; // default = 1000000
        std::map<int, std::string>  _error_page;     // empty if not in conf
        bool                        _root_in_conf;
        std::string                 _redirect_url;
        std::string                 _upload_path;
        void                        _test_upload_path();

    public :

        Location();
        Location(const std::string path, const std::string location_conf, const Location &general);
        Location(const Location &ref);
        ~Location();
        const Location &operator=(const Location &ref);

        std::string                         getPath() const;
        std::string                         getConf() const;
        std::string                         getRoot() const;
        std::vector<int>                    getAcceptedMethods() const;
        const std::list<std::string>        &getIndexes() const;
		bool                                getAutoIndex() const;
        std::string                         getCGIPath() const;
		size_t                              getMaxBodySize() const;
		const std::map<int, std::string>    &getErrorPage() const;
		std::string                         getErrorPath(int code) const;
        bool                                getRootInConf() const;
        std::string                         getRedirectURL() const;
        std::string                         getUploadPath() const;


        void    setRoot(const std::string &root);
        void    setIndexes(const std::list<std::string> &indexes);
        void    setAutoIndex(bool autoindex);
        void    setCGIpath(const std::string &cgi_path);
        void    setMaxBodySize(const std::string &error_page);
        // void    setErrorPage(const std::string &error_page);
        
        bool    isAcceptedMethod(int code) const;
};

#endif