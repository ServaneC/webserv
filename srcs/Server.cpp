/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:02:34 by schene            #+#    #+#             */
/*   Updated: 2021/06/08 20:28:43 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

Server::Server(Config &conf, std::string server_conf) : _rqst(*(new Request)), _main_conf(conf), _server_conf(server_conf)
{
    std::cout << "* CREATING SERVER *" <<std::endl;
    // (void)conf_file;
    // this->_conf.parseConfFile(conf_file.c_str());
    try
    {
        parsingIPAddress();
        // this->_port = 8080;
        std::cout << "- ServerPort = " << _port << std::endl;

        this->_name = parsingName();
        // this->_name = "localhost";
        std::cout << "- ServerName = " << _name << std::endl;

        this->_host.sin_family = PF_INET;
        this->_host.sin_addr.s_addr = INADDR_ANY; // -> 0.0.0.0
        // this->_host.sin_addr.s_addr = convertIPAddress();  // uncomment when function is ready
        // std::cout << "- ServerAddress = " << this->_host.sin_addr.s_addr << std::endl;
        this->_host.sin_port = htons(this->_port);
        this->_addrlen = sizeof(this->_host);
        this->_socket = socket(PF_INET, SOCK_STREAM, 0);
        
        parsingLocations();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl; }
    // std::cout << "* STARTING SERVER *" <<std::endl;
    // this->start_server();
    // std::cout << "* SUCCESS *" <<std::endl;
}

Server::~Server()
{
    delete &this->_rqst;
    delete &this->_main_conf;
    // free(&this->_rqst);
}

void	Server::start_server()
{    
    if (bind(this->_socket, (struct sockaddr *)&this->_host, this->_addrlen) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }    
    if (listen(this->_socket, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    fd_set current_sockets, read_sockets, write_sockets;

    //initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(this->_socket, &current_sockets);

    while (1)
    {
        read_sockets = current_sockets; //because select is destructive        
        write_sockets = current_sockets; //because select is destructive        
        if (select(FD_SETSIZE, &read_sockets, &write_sockets, NULL, NULL) < 0)
        {
            perror("In select");
            exit(EXIT_FAILURE);  
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &read_sockets))
            {
                std::cout << "isset write ? " << FD_ISSET(i, &write_sockets) << std::endl;
                if (i == this->_socket)
                {
                    if ((this->_client_socket = accept(this->_socket, (struct sockaddr *)&this->_host, (socklen_t*)&this->_addrlen))<0)
                    {
                        perror("In accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(this->_client_socket, &current_sockets);
                }
                else if (FD_ISSET(i, &write_sockets))
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

std::string Server::parsingName() const
{
    size_t name_index = _server_conf.find("server_name");
    if (name_index == std::string::npos)
        throw confNoServerNameException();  // a voir, je sais pas si un serveur doit forcement avoir un nom donné en config
    name_index += 12;
    // std::cout << "name index = |" << _server_conf[name_index] << "|" << std::endl;
    size_t name_length = _server_conf.find(";", name_index);
    if (name_length == std::string::npos)
        throw confNoServerNameException();
    name_length -= name_index;
    // std::cout << name_length << std::endl;
    return (_server_conf.substr(name_index, name_length));    
}

void Server::storeIPAddress(size_t index)
{
    _ipAddress.block1 = getValueBetweenPoints(_server_conf, &index);
    // std::cout << "BLOCK 1 = " << (int)_ipAddress.block1 << std::endl;
    _ipAddress.block2 = getValueBetweenPoints(_server_conf, &index);
    // std::cout << "BLOCK 2 = " << (int)_ipAddress.block2 << std::endl;
    _ipAddress.block3 = getValueBetweenPoints(_server_conf, &index);
    // std::cout << "BLOCK 3 = " << (int)_ipAddress.block3 << std::endl;
    _ipAddress.block4 = getValueBetweenPoints(_server_conf, &index);  
    // std::cout << "BLOCK 4 = " << (int)_ipAddress.block4 << std::endl;
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
    // std::cout << "port index = |" << _server_conf[port_index] << "|" << std::endl;
    size_t port_length = _server_conf.find(";", port_index);
    if (port_length == std::string::npos)
        throw confIPAddrException();
    port_length -= port_index;
    // std::cout << port_length << std::endl;
    this->_port = std::atoi(_server_conf.substr(port_index, port_length).c_str());
    if (this->_port < 0 || this->_port > 65535)
        throw confInvalidPortException();
}

int Server::convertIPAddress() const
{
    int ip = 0;
    // bitwise operations to convert my IPA_t to an int...
    return (ip);
}

void Server::parsingLocations()
{
    size_t last_found = _server_conf.find("location");

    while (last_found < _server_conf.size() && last_found < std::string::npos)
    {
        last_found += 9;
        std::string single_location = getScope(_server_conf, last_found, );
        std::cout << "GOT THIS INSTRUCTION FOR LOCATION : " << single_location << std::endl;
        if (!single_location.empty())
        {
            //wtf do i do with this ?? we'll see tomorrow
        }
        last_found = _server_conf.find("location", last_found);
    }
}

