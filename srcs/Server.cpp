/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/06/07 19:42:44 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

Server::Server(Config &conf, std::string server_conf) : _rqst(*(new Request)), _main_conf(conf), _server_conf(server_conf)
{
    // (void)conf_file;
    std::cout << "* CREATING SERVER *" <<std::endl;
    // this->_port = 8080;
    this->_port = parsingPort();
    std::cout << "- ServerPort = " << _port << std::endl;
    // this->_name = "localhost";
    this->_name = parsingName();
    std::cout << "- ServerName = " << _name << std::endl;

    this->_socket = socket(PF_INET, SOCK_STREAM, 0);
    this->_host.sin_family = PF_INET;
    this->_host.sin_addr.s_addr = INADDR_ANY; // -> 0.0.0.0
    this->_host.sin_port = htons(this->_port);
    this->_addrlen = sizeof(this->_host);

    int enable = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(this->_socket);
        exit(EXIT_FAILURE);  
    }
    if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl() failed");
        close(this->_socket);
        exit(EXIT_FAILURE); 
    }
    if (bind(this->_socket, (struct sockaddr *)&this->_host, this->_addrlen) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(this->_socket, 32) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    // std::cout << "* STARTING SERVER *" <<std::endl;
    //this->start_server();
    // std::cout << "* SUCCESS *" <<std::endl;
}

Server::~Server()
{
    close(this->_socket);
    delete &this->_rqst;
    delete &this->_main_conf;
    // free(&this->_rqst);
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
    this->_rqst.parseRequest(this->_client_socket);
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
        return ("NOPE");  // a voir, je sais pas si un serveur doit forcement avoir un nom donné en config
    name_index += 12;
    // std::cout << "name index = |" << _server_conf[name_index] << "|" << std::endl;
    size_t name_length = _server_conf.find(";", name_index);
    if (name_length == std::string::npos)
        return ("NOPE");
    name_length -= name_index;
    // std::cout << name_length << std::endl;
    return (_server_conf.substr(name_index, name_length));    
}

int Server::parsingPort() const
{
    size_t port_index = _server_conf.find("listen");
    if (port_index == std::string::npos)
        return (0);
    port_index = _server_conf.find(":", port_index);
    if (port_index == std::string::npos)
        return (0);
    port_index++;
    // std::cout << "port index = |" << _server_conf[port_index] << "|" << std::endl;
    size_t port_length = _server_conf.find(";", port_index);
    if (port_length == std::string::npos)
        return (0);
    port_length -= port_index;
    // std::cout << port_length << std::endl;
    return (std::atoi(_server_conf.substr(port_index, port_length).c_str()));
}
