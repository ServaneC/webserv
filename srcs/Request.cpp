/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/21 10:28:50 by schene           ###   ########.fr       */
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
	
	//while to "ignore at least one CRLF before request line"
	while  (get_next_line(this->_fd, &line) > 0) // parse request line
	{
		line_s = std::string(line);
		if (!(line_s[0] == '\r' || line_s.empty()))
		{
			std::cout << line_s << std::endl;
			this->parseRequestLine(line_s);
			break ;
		}
		if (line)
			free(line);
		line = NULL;
	}
	while (get_next_line(this->_fd, &line) > 0 && line) //parse header fields
	{
		line_s = std::string(line);
		std::cout << line_s << std::endl;

		std::string field = line_s.substr(0, line_s.find(':')); //parse field_name
		
		std::map<std::string, std::string>::iterator it = this->_headers.begin();
		while (it != this->_headers.end())
		{
			if (field.compare(it->first) == 0 && it->second.empty())
			{
				std::string value = line_s.substr(line_s.find(':') + 2, line_s.find('\r'));
				while (ft_isspace(value[0])) // handle OWS between ':' and field-value
					value.erase(0, 1);
				while (ft_isspace(*(value.end() - 1))) // handle OWS between field-value and CRLF
					value.erase(value.end() - 1);
				it->second = value;// store header value in map
				// std::cout << it->first << "|" << it->second << "|" << std::endl;
			}
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
	if (tmp.compare(0, 7, "http://") == 0)
	{
		tmp.erase(0 , 7);
		// std::cout << "tmp = " << tmp << std::endl;
		this->_headers["Host"] = tmp.substr(0, tmp.find('/')); //set Host
		tmp.erase(0, tmp.find('/'));
	}
	this->_target = tmp;
	line.erase(0, line.find(' ') + 1);
	this->_http_version = line.substr(0, line.find('\r')); //set http version
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

std::string const	&Request::getHeaderField(std::string field_name) const
{
	static std::string const	empty;

	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	while (it != this->_headers.end())
	{
		if (field_name.compare(it->first) == 0)
			return it->second;
		it++;
	}
	return empty;
}



