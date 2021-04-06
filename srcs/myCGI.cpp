/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myCGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 15:26:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myCGI.hpp"

myCGI::myCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest())
{
	this->_env["STATUS_CODE"] = std::string("200 OK");
	this->_env["AUTH_TYPE"] = std::string();
	this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	this->_env["GATEWAY_INTERFACE"] = std::string();
	this->setPathQuery();
	this->_env["REMOTE_ADDR"] = std::string();
	this->_env["REMOTE_IDENT"] = std::string();
	this->_env["REMOTE_USER"] = std::string();
	this->_env["REQUEST_METHOD"] = this->_request.getMethod();
	this->_env["REQUEST_URI"] = std::string();
	this->_env["SCRIPT_NAME"] = std::string();
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
	this->_env["SERVER_SOFTWARE"] = std::string();

	std::cout << "file = |" << this->_env["PATH_INFO"] << "|" << std::endl;
	this->_fd = open(this->_env["PATH_INFO"].c_str(), O_RDONLY);
	if (this->_fd < 0)
	{
        perror("open failed");
        exit(EXIT_FAILURE);
    }
	if (!(this->_buf = (char *)malloc(sizeof(char) * READ_SIZE)))
	{
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
	if (read(this->_fd, this->_buf, READ_SIZE) == -1)
	{
        perror("In read");
        exit(EXIT_FAILURE);
    }
	close(this->_fd);
	std::cout << "SUCCESS" << std::endl;
	Response((*this), this->_server.getSocket());
}

myCGI::~myCGI()
{
	// free(this->_buf);
}

void	myCGI::setPathQuery()
{
	std::string	target = this->_request.getTarget();
	this->_env["PATH_TRANSLATED"] = target;
	this->_env["PATH_INFO"] = target.substr(1, target.find('?'));
	this->_env["QUERY_STRING"] = std::string();
	if (target.find('?') != std::string::npos)
		this->_env["QUERY_STRING"] = target.substr(target.find('?'), target.size()); //maybe wrong if no '?'
}

std::string const	myCGI::getEnvVar(std::string var_name) const
{
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	while (it != this->_env.end())
	{
		if (var_name.compare(it->first) == 0)
			return it->second;
		it++;
	}
	return std::string();
}

std::string const 	myCGI::getBuf() const
{
	return std::string(this->_buf);
}


