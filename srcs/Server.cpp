/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/04/30 10:18:28 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

Server::Server(std::string conf_file) : _rqst(*(new Request)), _conf(*(new Config))
{
    (void)conf_file;
    this->_conf.parseConfFile(conf_file.c_str());
    this->_port = 8080;
    this->_name = "localhost";
    this->_host.sin_family = PF_INET;
    this->_host.sin_addr.s_addr = INADDR_ANY; // -> 0.0.0.0
    this->_host.sin_port = htons(this->_port);
    this->_addrlen = sizeof(this->_host);
    this->_socket = socket(PF_INET, SOCK_STREAM, 0);
    this->start_server();
}

Server::~Server()
{
    delete &this->_rqst;
    delete &this->_conf;
    // free(&this->_rqst);
}

void	Server::start_server()
{    
    if (bind(this->_socket, (struct sockaddr *)&this->_host, sizeof(this->_host)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }    
    if (listen(this->_socket, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    fd_set current_sockets, ready_sockets;

    //initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(this->_socket, &current_sockets);

    while (1)
    {
        ready_sockets = current_sockets; //because select is destructive        
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("In select");
            exit(EXIT_FAILURE);  
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == this->_socket)
                {
                    if ((this->_client_socket = accept(this->_socket, (struct sockaddr *)&this->_host, (socklen_t*)&this->_addrlen))<0)
                    {
                        perror("In accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(this->_client_socket, &current_sockets);
                }
                else
                {
                    fcntl(this->_client_socket, F_SETFL, O_NONBLOCK);
                    
                    this->_rqst.parseRequest(this->_client_socket);
                    execCGI((*this));
                    close(this->_client_socket);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }
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
    return this->_client_socket;
}

Request		&Server::getRequest() const
{
    return this->_rqst;
}

