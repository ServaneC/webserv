/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:45 by schene            #+#    #+#             */
/*   Updated: 2021/07/20 15:48:09 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Request.hpp"

Request::Request() : _bad_request(false), _body(NULL)
{

}

Request::~Request()
{
	this->_headers.clear();
	close(this->_socket);
}

int		Request::parseRequest(int socket)
{
	this->reset_body();
	this->_socket = socket;
	this->resetHeaders();

	this->recvHeader();
	if (this->_buf.empty())
		return (- 1);
	std::cout << "========= REQUEST =========" << std::endl;
	std::cout << this->_buf;
	std::cout << "===========================" << std::endl;

	if (gnlRequest() > 0)
		this->parseRequestLine(this->_line);
	while (gnlRequest() > 0)
	{
	 	if (this->_line[0] == '\r') //check if we are still in the headers fields
	 		break ;
	 	this->parseHeaderFields(this->_line);
	}
	this->_buf.clear();
	// if payload-body -> read it
	if (!this->getHeaderField("Content-Length").empty()) //request w/ a 'normal' body
		this->recvBody(std::atoi(this->_headers["Content-Length"].c_str()));
	else if (!this->getHeaderField("Transfer-Encoding").compare("chunked")) //chunk request
		recvChunk();
	write(1, this->_body, this->_body_size);
	std::cout << "========= END OF REQUEST =========" << std::endl;
	return 1;
}

int			Request::recvHeader() // recv byte per byte to stop at the end of the header fields
{

	unsigned char c = 0;
	while (this->_buf.find("\r\n\r\n") == std::string::npos &&
			this->_buf.find("\n\n") == std::string::npos)
	{
		c = this->recv_one();
		if (int(c) != 255)
			this->_buf.push_back(c);
		if (c == (unsigned char)-1)
		{
			for (int i = 0; i < 100; i++)
			{
				c = this->recv_one();
				if (c != (unsigned char)-1)
				{
					this->_buf.push_back(c);
					break ;
				}
			}
			if (c == (unsigned char)-1)
				return (-1);
		}
	}
		
	return 1;
}

int			Request::recvBody(int size)
{
	unsigned char *recv_buf;
	int  recv_ret;

	try {
		recv_buf = new unsigned char [size + 1];
	}
	catch (std::exception &e) {
        std::cout << e.what() << std::endl;
		return (-1);
	}
	memset(recv_buf, '\0', size);
	while (1)
	{
		recv_ret = recv(this->_socket, recv_buf, size, 0);
		if (recv_ret < 0)
			return (-1);
		if (recv_ret > 0)
		{
			recv_buf[recv_ret] = '\0';
			this->append_body(recv_buf, recv_ret);
			memset(recv_buf, '\0', recv_ret);
			if (recv_ret == size)
				break ;
		}
	}
	delete [] recv_buf;
	return 1;
}

int			Request::recvChunk()
{
	std::string		size_buf;
	int				size;
	unsigned char	buf;

	while (1)
	{
		size = 0;
		size_buf.clear();
		while (size_buf.find("\r\n") == std::string::npos)
			size_buf.push_back(recv_one());
		if (size_buf.size() > 2)
			size_buf.erase(size_buf.size() - 2, 2); // erase the CRLF
		size = hexa_to_int(size_buf);
		std::cout << "size = " << size << std::endl;
		//end of the chunk body
		if (size == 0)
		{
			while (recv_one() != (unsigned char)-1)
				;
			return 1;
		}
		// recv the chunk data + append to body
		for (int i = 0; i < size; i++)
		{
			buf = recv_one();
			this->append_body(&buf, 1);
			buf = 0;
		}
		// recv the CRLF
		recv_one();
		recv_one();
	}
	return -1;
}

unsigned char	Request::recv_one()
{
	unsigned char	c;
	int				recv_ret;

	while (1)
	{
		c = 0;
		recv_ret = recv(this->_socket, &c, 1, 0);
		// std::cout << '|' << recv_ret << '|' << std::endl;
		if (recv_ret < 0)
			return (-1);
		if (recv_ret > 0)
			break ;
	}
	return (c);
}


int			Request::append_body(unsigned char *buffer, int size)
{
	try {
		if (this->_body)
		{
			this->_body = MyRealloc(this->_body, this->_body_size, this->_body_size + size + 1);
			std::memmove(this->_body + this->_body_size, buffer, size);
		}
		else
		{
			this->_body = new unsigned char[size + 1];
			std::memmove(this->_body, buffer, size);
		}
		this->_body_size += size;
	}
	catch (std::exception &e) {
        std::cout << e.what() << std::endl;
		return (-1); }
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
	this->_object = _target.substr(_target.find_last_of("/") + 1, _target.size() - _target.find_last_of("/") - 1);
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
			it->second = value; // store header value in map
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

void		Request::reset_body()
{
	if (this->_body)
		delete [] this->_body;
	this->_body_size = 0;
	this->_body = NULL;
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

unsigned char 		*Request::getBody() const
{
	return this->_body;
}

int			 		Request::getBodySize() const
{
	return this->_body_size;
}

bool				Request::getBadRequest() const
{
	return this->_bad_request;
}

int					Request::getMethodCode() const
{
	return this->_method_code;
}

std::string	const	&Request::getDirPath() const
{
	return this->_dir_path;
}

std::string	const	&Request::getObject() const
{
	return this->_object;
}

void				Request::setDirPath(std::string path)
{
	this->_dir_path = path;
}

void				Request::setObject(std::string object)
{
	this->_object = object;
}
