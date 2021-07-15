/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/07/15 20:14:35 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

// Server::Server() {}

Server::Server(Config &conf, std::string server_conf) : 
    _rqst(*(new Request)), _main_conf(conf), _server_conf(server_conf),
    _client_socket(-1)//, _routes(1, Location(this, "/", server_conf))
{
    try
    {
        std::cout << "---------------------" << std::endl;

        parsingIPAddress(server_conf, &this->_ip, &this->_port);
        std::cout << "- ServerPort = " << _port << std::endl;

        this->_name = parsingName(server_conf);
        std::cout << "- ServerName = " << _name << std::endl;

        this->_routes = parsingLocations(server_conf);

        this->_root = findRootLocation(_routes);

        updateWithRootInfos(_routes, _root);
        
        // this->_root = parsingRoot(trimLocations(server_conf));
        // std::cout << "- ServerRoot = " << _root << std::endl;

        // this->_indexes = parsingIndexes(NULL, trimLocations(server_conf));
        // std::cout << "- Indexes = ";
        // for (std::list<std::string>::iterator it = _indexes.begin(); it != _indexes.end(); it++)
        //     std::cout << *it << " ";
        // std::cout << std::endl;

        // this->_autoindex = parsingAutoIndex(*this, trimLocations(server_conf));
        // std::cout << "- Autoindex -> " << _autoindex << std::endl;

        this->_host.sin_family = PF_INET;
        this->_host.sin_addr.s_addr = this->_ip;
        std::cout << "- ServerAddress = " << this->_host.sin_addr.s_addr << std::endl;
        this->_host.sin_port = htons(this->_port);
        this->_addrlen = sizeof(this->_host);
        this->_socket = socket(PF_INET, SOCK_STREAM, 0);


        int enable = 1;
        if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            throw 20;
        if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
            throw 20;
        if (bind(this->_socket, (struct sockaddr *)&this->_host, this->_addrlen) < 0)
            throw 20;
        if (listen(this->_socket, 32) < 0)
            throw 20;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl; }
}

Server::~Server()
{
    close(this->_socket);
    delete &this->_rqst;
    delete &this->_main_conf;
    for (std::list<Location>::iterator it = _routes.begin(); it != _routes.end(); it++)
        // delete &it;   // jamais testé mais je pense que c'est de la merde
        delete &(*it);   // ca c'est ptet mieux
    // {                  // sinon ca ??
    //     Location *loc = &(*it);
    //     delete loc;
    // }
}

int 	Server::exec_accept()
{    
        if ((this->_client_socket = accept(this->_socket, (struct sockaddr *)&this->_host, (socklen_t*)&this->_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        return (this->_client_socket);
}

int 	Server::exec_server()
{
    fcntl(this->_client_socket, F_SETFL, O_NONBLOCK);
    if (this->_rqst.parseRequest(this->_client_socket) < 0)
        return (-1);
    execCGI((*this));
    close(this->_client_socket);
    this->_client_socket = -1; 
    return 1;
}

int			Server::getPort() const
{
    return this->_port;
}

std::string	Server::getName() const
{
    return this->_name;
}
std::string	Server::getRoot() const
{
    return this->_root->getRoot();
}
const Location	&Server::getRootLocation() const
{
    return *(this->_root);
}
int			Server::getFd() const
{
    return this->_socket;
}

int			Server::getSocket() const
{
    return this->_socket;
}

int			Server::getClientSocket() const
{
    return this->_client_socket;
}

Request		&Server::getRequest() const
{
    return this->_rqst;
}

const std::list<Location> &Server::getRoutes() const
{
    return this->_routes;
}

// const std::list<std::string>	&Server::getIndexes() const
// {
//     return (this->_indexes);
// }

// bool					Server::getAutoIndex() const
// {
//     return (this->_autoindex);
// }