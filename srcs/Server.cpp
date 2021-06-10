/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/06/09 03:27:01 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

Server::Server(Config &conf, std::string server_conf) : _rqst(*(new Request)), _main_conf(conf), _server_conf(server_conf)
{
    try
    {
        std::cout << "---------------------" << std::endl;
        parsingIPAddress();
        std::cout << "- ServerPort = " << _port << std::endl;

        this->_name = parsingName();
        std::cout << "- ServerName = " << _name << std::endl;

        this->_host.sin_family = PF_INET;
        // this->_host.sin_addr.s_addr = INADDR_ANY; // -> 0.0.0.0
        this->_host.sin_addr.s_addr = this->_ipAddress.address;
        std::cout << "- ServerAddress = " << this->_host.sin_addr.s_addr << std::endl;
        this->_host.sin_port = htons(this->_port);
        this->_addrlen = sizeof(this->_host);
        this->_socket = socket(PF_INET, SOCK_STREAM, 0);
        parsingLocations();

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

void	Server::exec_server()
{
    fcntl(this->_client_socket, F_SETFL, O_NONBLOCK);
    if (this->_rqst.parseRequest(this->_client_socket) > 0)
        execCGI((*this));
    close(this->_client_socket);
}

int			Server::getPort() const
{
    return this->_port;
}

std::string	Server::getName() const
{
    return this->_name;
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

std::string Server::parsingName() const
{
    size_t name_index = _server_conf.find("server_name");
    if (name_index == std::string::npos)
        throw confNoServerNameException();  // a voir, je sais pas si un serveur doit forcement avoir un nom donné en config
    name_index += 12;
    size_t name_length = _server_conf.find(";", name_index);
    if (name_length == std::string::npos)
        throw confNoServerNameException();
    return (_server_conf.substr(name_index, name_length - name_index));
}

void Server::storeIPAddress(size_t index)
{
    for (int i = 3; i > -1; i--)
        _ipAddress.block[i] = getValueBetweenPoints(_server_conf, &index);
}

void Server::parsingIPAddress()
{
    size_t addr_index = _server_conf.find("listen");
    if (addr_index == std::string::npos)
        throw confNoListenException();
    addr_index = _server_conf.find_first_not_of(" \t\n\r\v\f", addr_index + 7);
    storeIPAddress(addr_index);
    size_t port_index = _server_conf.find(":", addr_index);
    if (port_index == std::string::npos)
        throw confIPAddrException();
    port_index++;
    size_t port_length = _server_conf.find(";", port_index);
    if (port_length == std::string::npos)
        throw confIPAddrException();
    this->_port = std::atoi(_server_conf.substr(port_index, port_length - port_index).c_str());
    if (this->_port < 0 || this->_port > 65535)
        throw confInvalidPortException();
}

void Server::parsingLocations()
{
    size_t last_found = _server_conf.find("location");

    while (last_found < _server_conf.size() && last_found < std::string::npos)
    {
        last_found += 9;
        size_t path_index = _server_conf.find_first_not_of(" \t\n\r\v\f", last_found);
        size_t path_length = _server_conf.find_first_of(" \t\n\r\v\f", path_index) - path_index;
        std::string path = _server_conf.substr(path_index, path_length);
        std::string rules = getScope(_server_conf, path_index + path_length);
        if (!rules.empty())
        {
            _routes[path] = rules;
            //std::cout << "FOR LOCATION <" << path << ">, RULES ARE : " << rules << std::endl;
            //wtf do i do with this ?? we'll see tomorrow
        }
        last_found = _server_conf.find("location", last_found);
    }
}
