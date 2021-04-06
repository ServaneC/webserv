/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/04/05 16:07:59 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string conf_file) : _rqst(*(new Request))
{
    (void)conf_file;
    this->_port = 8080;
    this->_name = "localhost";
    this->_host.sin_family = PF_INET;
    this->_host.sin_addr.s_addr = INADDR_ANY;
    // this->_host.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->_host.sin_port = htons(this->_port);
    this->_addrlen = sizeof(this->_host);
    this->_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (bind(this->_fd, (struct sockaddr *)&this->_host, sizeof(this->_host)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }    
    if (listen(this->_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nHello c'est schene!";
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((this->_socket = accept(this->_fd, (struct sockaddr *)&this->_host, (socklen_t*)&this->_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        fcntl(this->_socket, F_SETFL, O_NONBLOCK);
        this->_rqst.parseRequest(this->_socket);
        if (this->_rqst.getTarget().empty())
        {
            perror("empty request");
            exit(EXIT_FAILURE);
        }
        std::cout << "target = |" << this->_rqst.getTarget() << '|' << std::endl;
        // if (this->_rqst.getTarget().compare("test.html") == 0)
        myCGI((*this));
        // else
        //     send(this->_socket, hello.c_str(), hello.size(), 0);
        // write(this->_socket , hello.c_str() , hello.size());
        // printf("------------------Hello message sent-------------------\n");
        close(this->_socket);
    }
    //unlink(this->_host.sun_path);
}

Server::~Server()
{
    free(&this->_rqst);
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
    return this->_fd;
}

int			Server::getSocket() const
{
    return this->_socket;
}

Request		&Server::getRequest() const
{
    return this->_rqst;
}

