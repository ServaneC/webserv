/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:42:45 by schene            #+#    #+#             */
/*   Updated: 2021/05/06 14:45:04 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Response.hpp"

Response::Response(execCGI &my_CGI, int socket) :  
	_cgi(my_CGI), _socket(socket), _buf(my_CGI.getBuf())
{
	// my_CGI.free_buf();
	this->_headers["Status"] = my_CGI.getEnvVar("STATUS_CODE");
	this->parse_cgi_buf();
	this->_headers["Allow"] = std::string();
	this->_headers["Content-Language"] = std::string();
	this->_headers["Content-Length"] = this->itoa_cpp(this->_buf.size());
	this->_headers["Content-Location"] = std::string();
	//this->setDate();
	//this->setLastModified();
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
	std::cout << "cgi buf [" << this->_buf << ']' << std::endl;
	if (this->_buf.empty() || this->_buf[0] == '<')
		return ;
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

void		Response::setDate()
{
	time_t			date;
	
	time(&date);
	this->_headers["Date"] = this->formatDate(date);
}

void			Response::setLastModified()
{
	if (this->_cgi.getLastModified())
		this->_headers["Last-Modified"] = this->formatDate(this->_cgi.getLastModified());
}

std::string		Response::formatDate(time_t timestamp)
{
	//GMT time is 2 hour behind localtime in france (summer hour otherwhise it's 1 hour behind)
	char			buffer[30];
 
	struct tm tm = *gmtime(&timestamp);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	return std::string(buffer);
}

void			Response::writeStatusLine()
{
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	this->_response = this->_cgi.getEnvVar("SERVER_PROTOCOL") + ' '; // HTTP-Version SP 
	this->_response += this->_headers.find("Status")->second + "\r\n"; // Status-Code SP Reason-Phrase CRLF
	this->_headers.erase("Status"); // Erase status from header to not print it as a header field
}

void			Response::format_header()
{
	// header-field = field-name ":" OWS field-value OWS CRLF
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	for (; it != this->_headers.end(); it++) // loop to print all headers fields
	{
		if (!(it->second.empty()))
		{
			std::string tmp = it->first + ": " + this->_headers.find(it->first)->second + "\r\n";
			this->_response.append(tmp);
		}
	}
}

void			Response::send_response()
{
	// status line
	this->writeStatusLine();
	// Headers
	this->format_header();
	//CRLF to separate header and body (or end)
	this->_response.append("\r\n"); 
	// Message-Body
	if (!this->_buf.empty())
	{
		// std::cout << "buf = |" << this->_buf << "|" << std::endl;
		this->_response.append(this->_buf);
	}
	//print on our cout to check
	std::cout << "------------ RESPONSE -----------------" << std::endl;
	std::cout << this->_response;
	std::cout << "---------------------------------------" << std::endl;
	// Actually sending to socket
	send(this->_socket, this->_response.c_str(), this->_response.size(), 0);
	this->_cgi.free_buf();
}

std::string	Response::itoa_cpp(int n)
{
	std::ostringstream convert;
	convert << n;
	return (convert.str());
}
