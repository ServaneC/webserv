/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:42:45 by schene            #+#    #+#             */
/*   Updated: 2021/04/05 15:52:00 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(myCGI &my_CGI, int socket) :  _cgi(my_CGI), _socket(socket)
{
	this->_buf_file = my_CGI.getBuf();
	this->_headers["Allow"] = std::string();
	this->_headers["Content-Language"] = std::string();
	char *tmp = ft_itoa(this->_buf_file.size());
	this->_headers["Content-Length"] = std::string(tmp);
	this->_headers["Content-Location"] = std::string();
	this->_headers["Content-Type"] = std::string("text/html");
	this->setDate();
	this->setLastModified();
	// this->_headers["Last-Modified"] = std::string(); // need to use stat fct
	this->_headers["Location"] = std::string();
	this->_headers["Retry-After"] = std::string();
	this->_headers["Server"] = std::string("webserv/1.0");
	this->_headers["Transfer-Encoding"] = std::string();
	this->_headers["WWW-Authenticate"] = std::string();
	
	this->_response = my_CGI.getEnvVar("SERVER_PROTOCOL") + ' ' + my_CGI.getEnvVar("STATUS_CODE") + '\n';
	//this->print_header("Date");
	this->_response.append(this->search_header("Date"));
	this->_response.append(this->search_header("Content-Length"));
	this->_response.append(this->search_header("Content-Type"));
	this->_response.append(this->search_header("Last-Modified"));

	// this->print_header("Content-Type");
	this->_response.append("\n");
	this->_response.append(this->_buf_file);
	// std::cout << "\n";

	send(this->_socket, this->_response.c_str(), this->_response.size(), 0);
	// write(1, this->_buf_file.c_str(), this->_buf_file.size());
}

Response::~Response() {}

std::string		Response::search_header(std::string field_name) const
{
	std::string tmp = std::string();
	tmp = field_name + ": " + this->_headers.find(field_name)->second + "\n";
	return (tmp);
	// this->_response.append(tmp);
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

std::string		Response::formatDate(time_t timestamp)
{
	char			buffer[30];
	struct tm tm = *gmtime(&timestamp);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	return std::string(buffer);
}

