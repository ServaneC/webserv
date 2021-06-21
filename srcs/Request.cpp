/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/06/21 16:48:34 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Request.hpp"

#define CHUNK_SIZE 2000
#define HEADER 1
#define BODY 2

Request::Request() : _bad_request(false)
{

}

Request::~Request()
{
	this->_headers.clear();
	close(this->_socket);
}

int		Request::parseRequest(int socket)
{
	this->_socket = socket;
	this->resetHeaders();

	if (this->recvData(CHUNK_SIZE, HEADER) < 0)
		return -1;

	if (this->_buf.empty())
		return (- 1);
	std::cout << "========= REQUEST =========" << std::endl;
	std::cout << this->_buf;
	std::cout << "==================" << std::endl;

	//while because "ignore at least one CRLF before request line"
	while (gnlRequest() > 0)
	{
		if (!(this->_line[0] == '\r' || this->_line.empty()))
		{
			this->parseRequestLine(this->_line);
			break ;
		}
	}
	while (gnlRequest() > 0)
	{
	 	if (this->_line.find(':') == std::string::npos) //check if we are still in the headers fields
	 		break ;
	 	this->parseHeaderFields(this->_line);
	}
	// //check if CRLF or LF (no CRLF or LF = error) CRLF -> line = "\r" LF = line = empty
	if (!(this->_line.empty() || (this->_line[0] == '\r' && !this->_line[1])))
	 	this->_bad_request = true;
	// if payload-body -> read it
	this->_body.clear();
	this->_body.assign(this->_buf);
	if (!this->getHeaderField("Content-Length").empty())
	{
		// while (this->_body.empty())
		while ((int)this->_body.size() < std::atoi(this->getHeaderField("Content-Length").c_str()))
			this->recvData(std::atoi(this->_headers["Content-Length"].c_str()), BODY);
	}
	this->_buf.clear();
	// std::cout << this->_body;
	std::cout << "========= END OF REQUEST =========" << std::endl;
	return 1;
}

int			Request::recvData(int size, int mode)
{
	char *recv_buf;
	int  recv_ret;

	try {
		recv_buf = new char [size + 1];
	}
	catch (std::exception &e) {
        std::cout << e.what() << std::endl;
		return (-1);
	}
	memset(recv_buf, '\0', size);
	while ((recv_ret = recv(this->_socket, recv_buf, size, 0)) > 0)
	{
		recv_buf[recv_ret] = '\0';
		if (mode == HEADER)
			this->_buf.append((const char *)recv_buf);
		else if (mode == BODY)
			this->_body.append((const char *)recv_buf);
		memset(recv_buf, '\0', recv_ret);
		if (recv_ret < size || mode == BODY)
			break ;
	}
	delete [] recv_buf;
	return 1;
}

int Request::gnlRequest()
{
	size_t pos = this->_buf.find_first_of('\n');

	this->_line.clear();
	if (pos != std::string::npos)
	{
		this->_line = this->_buf.substr(0, pos);
		this->_buf.erase(0, pos + 1);
		return pos;
	}
	return (-1);
}

void		Request::parseRequestLine(std::string line)
{
	// request-line = method SP request-target SP HTTP-version CRLF
	this->_method = line.substr(0, line.find(' ')); // parse method
	this->_method_code = setMethodCode(this->_method);
	line.erase(0, line.find(' ') + 1);
	if (isspace(line[0]))
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
	if (isspace(line[0]))
		this->_bad_request = true;
	this->_http_version = line.substr(0, line.find('\r')); //set http version
}

void		Request::parseHeaderFields(std::string line)
{
	std::string field_name = line.substr(0, line.find(':')); //parse field_name
		
	if (isspace(field_name[field_name.size() - 1])) //No whitespace is allowed between the header field-name and colon
		this->_bad_request = true;
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	while (it != this->_headers.end())
	{
		if (field_name.compare(it->first) == 0)
		{
			std::string value = line.substr(line.find(':') + 2, line.find('\r'));
			while (isspace(value[0])) // handle OWS between ':' and field-value
				value.erase(0, 1);
			while (isspace(*(value.end() - 1))) // handle OWS between field-value and CRLF
				value.erase(value.end() - 1);
			it->second = value;// store header value in map
			// std::cout << it->first << " = " << it->second << std::endl;
		}
		it++;
	}
}

void		Request::resetHeaders()
{
	this->_headers["Accept"] = std::string();
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


char		*Request::free_null_line(char *line)
{
	if (line)
		free(line);
	line = NULL;
	return line;
}

// GETTER
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

std::string const	&Request::getBody() const
{
	return this->_body;
}

bool				Request::getBadRequest() const
{
	return this->_bad_request;
}

int					Request::getMethodCode() const
{
	return this->_method_code;
}
