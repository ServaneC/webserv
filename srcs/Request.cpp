/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/06 09:32:23 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	this->_headers["Accept-Charsets"] = std::string();
	this->_headers["Accept-Language"] = std::string();
	this->_headers["Authorization"] = std::string();
	this->_headers["Content-Length"] = std::string();
	this->_headers["Content-Type"] = std::string();
	this->_headers["Date"] = std::string();
	this->_headers["Host"] = std::string();
	this->_headers["Referer"] = std::string();
	this->_headers["Transfer-Encoding"] = std::string();
	this->_headers["User-Agent"] = std::string();
}

Request::~Request()
{
	this->_headers.clear();
	close(this->_fd);
}

// Request 	&Request::operator=(Request const &rhs)
// {
// 	*this = rhs;
// 	return *this;
// }

int		Request::parseRequest(int fd)
{
	char		*line;

	this->_fd = fd;
	std::string line_s;
	
	if (get_next_line(this->_fd, &line) > 0) //parse request line
	{
		line_s = std::string(line);
		// std::cout  << "l = |" << line_s << '|' << std::endl;
		this->_method = line_s.substr(0, line_s.find(' '));
		// std::cout <<  "line = |" << line_s<< '|' << std::endl;
		line_s.erase(0, line_s.find(' ') + 1);
		this->_target = line_s.substr(0, line_s.find(' '));
		line_s.erase(0, line_s.find(' ') + 1);
		this->_http_version = line_s;
		free(line);
		line = NULL;
	}
	while (get_next_line(this->_fd, &line) > 0 && line) //parse header fields
	{
		line_s = std::string(line);
		// std::cout  << "l = |" << line_s << '|' << std::endl;
		std::string field = line_s.substr(0, line_s.find(':'));
		
		std::map<std::string, std::string>::iterator it = this->_headers.begin();
		while (it != this->_headers.end())
		{
			if (field.compare(it->first) == 0)
				it->second = line_s.substr(line_s.find(':') + 2, line_s.size());
			// std::cout << it->first << " => " << it->second << std::endl;
			it++;
		}
		free(line);
		line = NULL;
		// std::cout << "----------------" << std::endl;
	}
	//if Content-Length or Transfer-Encoding Header => Message Body after
	return 1;
}

std::string const	&Request::getMethod() const
{
	return this->_method;
}

std::string const	&Request::getTarget() const
{
	return this->_target;
}

std::string const	&Request::getHTTPVersion() const
{
	return this->_http_version;
}

std::string const	Request::getHeaderField(std::string field_name) const
{
	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	while (it != this->_headers.end())
	{
		if (field_name.compare(it->first) == 0)
			return it->second;
		it++;
	}
	return std::string();
}



