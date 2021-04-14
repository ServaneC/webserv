/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:42:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/14 11:35:33 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Response.hpp"

Response::Response(myCGI &my_CGI, int socket) :  
	_cgi(my_CGI), _socket(socket), _buf(my_CGI.getBuf())
{
	this->_headers["Status"] = my_CGI.getEnvVar("STATUS_CODE");
	this->parse_cgi_buf();
	this->_headers["Allow"] = std::string();
	this->_headers["Content-Language"] = std::string();
	char *tmp = ft_itoa(this->_buf.size());
	this->_headers["Content-Length"] = std::string(tmp);
	this->_headers["Content-Location"] = std::string();
	this->setDate();
	this->setLastModified();
	this->_headers["Location"] = std::string();
	this->_headers["Retry-After"] = std::string();
	this->_headers["Server"] = std::string("webserv/1.0");
	this->_headers["Transfer-Encoding"] = std::string();
	this->_headers["WWW-Authenticate"] = std::string();
	this->send_response();
}

Response::~Response() // memory reuse ?
{
	this->_headers.clear();
}

void		Response::parse_cgi_buf()
{
	while (this->_buf.find(':') != std::string::npos)
	{
		if (this->_buf[0] == '\r') // end of the header 
			break ;
		std::string tmp = this->_buf.substr(0, this->_buf.find('\n') - 1); //line with the header field
		this->_buf.erase(0, this->_buf.find('\n') + 1); // erase the line from the buf
		std::string name = tmp.substr(0, tmp.find(':')); // separate the field-name
		if (name.compare("Content-type"))
			name = "Content-Type";
		this->_headers[name] = tmp.substr(tmp.find(':') + 2, tmp.size()); // put the name and value in the _header map
	}
}

void			Response::send_response() // here print = add to response string
{
	this->_response = this->_cgi.getEnvVar("SERVER_PROTOCOL") + ' ';
	this->_response += this->_headers.find("Status")->second + "\r\n"; // print status line
	this->_headers.erase("Status"); // Erase status from header to not print it as a header field
	std::map<std::string, std::string>::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++) // loop to print all headers fields
	{
		if (!(it->second.empty()))
			this->_response.append(this->search_header(it->first));
	}
	// this->_response.append("\r\n"); //CRLF to separate header and body (commented bc  already in buf)
	this->_response.append(this->_buf); // print body
	send(this->_socket, this->_response.c_str(), this->_response.size(), 0); //we send the response 
}

std::string		Response::search_header(std::string field_name) const
{
	std::string tmp = std::string();
	tmp = field_name + ": " + this->_headers.find(field_name)->second + "\r\n";
	return (tmp);
}

void		Response::setDate()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	this->_headers["Date"] = this->formatDate(tv.tv_sec);
}

void			Response::setLastModified()
{
	std::string path = this->_cgi.getEnvVar("PATH_INFO");
	struct stat info;

	lstat(path.c_str(), &info);
	this->_headers["Last-Modified"] = this->formatDate(info.st_mtime);
}

std::string		Response::formatDate(time_t timestamp) // ‼️ gmtime is not an authorized function 
{
	char			buffer[30];
	struct tm tm = *gmtime(&timestamp);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	return std::string(buffer);
}

