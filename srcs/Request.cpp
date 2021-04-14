/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/14 16:07:59 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Request.hpp"

Request::Request() //init header map
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

int		Request::parseRequest(int fd)
{
	char		*line;

	this->_fd = fd;
	std::string line_s;
	
	if (get_next_line(this->_fd, &line) > 0) //parse request line
	{
		line_s = std::string(line);
		this->parseRequestLine(line_s);
		// this->_method = line_s.substr(0, line_s.find(' ')); // parse method
		// line_s.erase(0, line_s.find(' ') + 1);
		// this->_target = line_s.substr(0, line_s.find(' ')); //parse target
		// line_s.erase(0, line_s.find(' ') + 1);
		// this->_http_version = line_s.substr(0, line_s.find('\r')); //parse http-version
		free(line);
		line = NULL;
	}
	while (get_next_line(this->_fd, &line) > 0 && line) //parse header fields
	{
		line_s = std::string(line);
		std::string field = line_s.substr(0, line_s.find(':')); //parse field_name
		
		std::map<std::string, std::string>::iterator it = this->_headers.begin();
		while (it != this->_headers.end())
		{
			if (field.compare(it->first) == 0)
				it->second = line_s.substr(line_s.find(':') + 2, line_s.find('\r')); // store header value in map
			it++;
		}
		free(line);
		line = NULL;
	}
	//if Content-Length or Transfer-Encoding Header => Message Body after
	return 1;
}

void		Request::parseRequestLine(std::string line)
{
	this->_method = line.substr(0, line.find(' ')); // parse method
	line.erase(0, line.find(' ') + 1);
	std::string tmp = line.substr(0, line.find(' ')); //parse target
	// this->_target = line.substr(0, line.find(' ')); //parse target
	if (tmp.compare(0, 7, "http://") == 0)
	{
		tmp.erase(0 , 7);
		std::cout << "tmp = " << tmp << std::endl;
		this->_headers["Host"] = tmp.substr(0, tmp.find('/'));
		tmp.erase(0, tmp.find('/'));
	}
	this->_target = tmp;
	line.erase(0, line.find(' ') + 1);
	this->_http_version = line.substr(0, line.find('\r')); 
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



