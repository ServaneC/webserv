/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/28 13:19:16 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Request.hpp"

Request::Request() : _bad_request(false) //init header map
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
	char		*line = NULL;
	
	this->_bad_request = false;
	this->_fd = fd;
	std::string line_s;
	
	//while because "ignore at least one CRLF before request line"
	while (get_next_line(this->_fd, &line) > 0) // parse request line
	{
		line_s = std::string(line);
		if (!(line_s[0] == '\r' || line_s.empty()))
		{
			this->parseRequestLine(line_s);
			break ;
		}
		line = this->free_null_line(line);
	}
	while (get_next_line(this->_fd, &line) > 0) //parse header fields
	{
		line_s = std::string(line);
		if (line_s.find(':') == std::string::npos) //check if we are still in the headers fields
			break ;
		this->parseHeaderFields(line_s);
		line = this->free_null_line(line);
	}
	//check if CRLF (no CRLF = error) CRLF -> line = "\r"
	if (!line || (line && ((line[0] != '\r')  || line[1])))
		this->_bad_request = true;
	line = this->free_null_line(line);
	// if payload-body -> read it (need testing)
	if (!this->getHeaderField("Content-Length").empty() || 
			!this->getHeaderField("Transfer-Encoding").empty())
		//if content-lenght, maybe just do a read of the length ? or at least check
	{
		while (1)
		{
			int ret = get_next_line(this->_fd, &line);
			if (line)
			{
				this->_body.append(std::string(line));
				if (ret > 0)
					this->_body.append("\n");
				line = this->free_null_line(line);
			}
			if (ret == -1)
				break ;
		}
	}
	return 1;
}

void		Request::parseRequestLine(std::string line)
{
	// request-line = method SP request-target SP HTTP-version CRLF
	this->_method = line.substr(0, line.find(' ')); // parse method
	line.erase(0, line.find(' ') + 1);
	if (ft_isspace(line[0]))
		this->_bad_request = true;
	std::string tmp = line.substr(0, line.find(' ')); //parse request-target
	if (tmp.compare(0, 7, "http://") == 0)
	{
		tmp.erase(0 , 7);
		this->_headers["Host"] = tmp.substr(0, tmp.find('/')); //set Host
		tmp.erase(0, tmp.find('/'));
	}
	this->_target = tmp;
	line.erase(0, line.find(' ') + 1);
	if (ft_isspace(line[0]))
		this->_bad_request = true;
	this->_http_version = line.substr(0, line.find('\r')); //set http version
}

void		Request::parseHeaderFields(std::string line)
{
	std::string field_name = line.substr(0, line.find(':')); //parse field_name
		
	if (ft_isspace(field_name[field_name.size() - 1])) //No whitespace is allowed between the header field-name and colon
		this->_bad_request = true;
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	while (it != this->_headers.end())
	{
		if (field_name.compare(it->first) == 0 && it->second.empty())
		{
			std::string value = line.substr(line.find(':') + 2, line.find('\r'));
			while (ft_isspace(value[0])) // handle OWS between ':' and field-value
				value.erase(0, 1);
			while (ft_isspace(*(value.end() - 1))) // handle OWS between field-value and CRLF
				value.erase(value.end() - 1);
			it->second = value;// store header value in map
		}
		it++;
	}
}

char		*Request::free_null_line(char *line)
{
	if (line)
		free(line);
	line = NULL;
	return line;
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

bool				Request::getBadRequest() const
{
	return this->_bad_request;
}


