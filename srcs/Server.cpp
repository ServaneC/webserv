/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/06/10 16:03:25 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

Server::Server(Config &conf, std::string server_conf) : _rqst(*(new Request)), _main_conf(conf), _server_conf(server_conf)
{
    try
    {
        std::cout << "---------------------" << std::endl;

        parsingIPAddress(server_conf, &this->_ip, &this->_port);
        // this->_port = 8080;
        std::cout << "- ServerPort = " << _port << std::endl;

        this->_name = parsingName(server_conf);
        // this->_name = "localhost";
        std::cout << "- ServerName = " << _name << std::endl;

        this->_host.sin_family = PF_INET;
        // this->_host.sin_addr.s_addr = INADDR_ANY; // -> 0.0.0.0
        this->_host.sin_addr.s_addr = this->_ip;
        std::cout << "- ServerAddress = " << this->_host.sin_addr.s_addr << std::endl;
        this->_host.sin_port = htons(this->_port);
        this->_addrlen = sizeof(this->_host);
        this->_socket = socket(PF_INET, SOCK_STREAM, 0);
        this->_routes = parsingLocations(server_conf);

        int enable = 1;
        setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        fcntl(this->_socket, F_SETFL, O_NONBLOCK);
        bind(this->_socket, (struct sockaddr *)&this->_host, this->_addrlen);
        listen(this->_socket, 32);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl; }

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
