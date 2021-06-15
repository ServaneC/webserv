/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:42:45 by schene            #+#    #+#             */
/*   Updated: 2021/06/15 15:52:35 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Response.hpp"

Response::Response(execCGI &my_CGI, int socket) :  
	_cgi(my_CGI), _socket(socket), _idx(0)
{
	this->_idx = 0;
	if (this->_cgi.getBuf())
	{
		this->_buf = NULL;
		// this->_bufb = "NULL";
		this->_buf_size = this->_cgi.getBufSize();
		// write(1, this->_cgi.getBuf(), this->_cgi.getBufSize());
		// this->_buf = new char[this->_buf_size + 1];
		// this->_buf = std::strcpy(this->_buf, this->_cgi.getBuf());
		this->_bufb = std::string(this->_cgi.getBuf());
		//this->_cgi.free_buf();
	}
		
	this->_headers["Status"] = this->_cgi.getEnvVar("STATUS_CODE");
	this->parse_cgi_buf();
	this->_headers["Allow"] = std::string();
	this->_headers["Content-Language"] = std::string();
	std::cout << "SIZE = " << this->_bufb.size() << std::endl;
	this->_headers["Content-Length"] = ft_itoa_cpp(this->_bufb.size());
	// this->_headers["Content-Length"] = ft_itoa_cpp(this->_buf_size);
	this->_headers["Content-Location"] = std::string();
	this->setDate();
	this->setLastModified();
	this->_headers["Location"] = std::string();
	this->_headers["Retry-After"] = std::string();
	this->_headers["Server"] = std::string("webserv/4.2");
	this->_headers["Transfer-Encoding"] = std::string();
	this->_headers["WWW-Authenticate"] = std::string();
	this->check_content_type();
	this->send_response();
}

Response::~Response()
{
	this->_headers.clear();
}

// void		Response::parse_cgi_buf()
// {
// 	if (!this->_buf || this->_buf[0] == '<')
// 		return ;
// 	while (strchr(&this->_buf[this->_idx], ':')) // check if there is still header-fields
// 	{
// 		if ((&this->_buf[this->_idx])[0] == '\r') // end of the header 
// 			break ;
// 		int next_n = std::strcspn(&this->_buf[this->_idx], "\n");
// 		std::cout << "span = " << next_n << std::endl;
// 		std::string tmp_s = std::string((&this->_buf[this->_idx]), next_n - 1); //line with the header field (maybe -2)
		
// 		// erase the line from buf
// 		this->_idx += next_n + 1;
// 		std::cout << "idx = " << this->_idx << std::endl;
// 		// char *tmp = new char[this->_buf_size - next_n + 1];
// 		// tmp = std::strncpy(tmp, &this->_buf[next_n + 1], this->_buf_size - next_n);
// 		// std::memset(this->_buf, 0, this->_buf_size);
// 		// this->_buf_size -= next_n;
// 		// this->_buf = std::strcpy(this->_buf, tmp);
// 		// delete [] tmp;

// 		std::string name = tmp_s.substr(0, tmp_s.find(':')); // separate the field-name
// 		if (name[0] != 'X')
// 		{
// 			if (name.compare("Content-type") == 0)
// 				name = "Content-Type";
// 			this->_headers[name] = tmp_s.substr(tmp_s.find(':') + 2, tmp_s.size()); // put the name and value in the _header map
// 			std::cout << '|' << name << " = " << this->_headers[name] << '|' << std::endl;
// 		}
// 	}
// }

void		Response::parse_cgi_buf()
{
	// std::cout << "CGI BUF (" << this->_bufb << ')' << std::endl;
	if (this->_bufb.empty() || this->_bufb[0] == '<')
		return ;
	while (this->_bufb.find(':') != std::string::npos)
	{
		if (this->_bufb[0] == '\r') // end of the header 
		{
			this->_bufb.erase(0, this->_bufb.find('\n') + 1);
			break ;
		}	
		std::string tmp = this->_bufb.substr(0, this->_bufb.find('\n') - 1); //line with the header field
		this->_bufb.erase(0, this->_bufb.find('\n') + 1); // erase the line from the buf
		std::string name = tmp.substr(0, tmp.find(':')); // separate the field-name
		if (name[0] != 'X')
		{
			if (name.compare("Content-type") == 0)
				name = "Content-Type";
			this->_headers[name] = tmp.substr(tmp.find(':') + 2, tmp.size()); // put the name and value in the _header map
		}
	}
	this->_bufb = (char *)this->_bufb.c_str();
}

void		Response::setDate()
{
	time_t			date;
	
	time(&date);
	this->_headers["Date"] = this->formatDate(date);
}

void			Response::setLastModified()
{
	if (this->_cgi.getLastModified() > 0)
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
	// std::string line;
	// line = this->_cgi.getEnvVar("SERVER_PROTOCOL") + ' ';
	// line += this->_headers.find("Status")->second + "\r\n";
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

void		Response::check_content_type()
{
	std::string tmp(this->_cgi.getEnvVar("PATH_INFO"));
	if (tmp.find('.') != std::string::npos)
	{
		tmp.erase(0, tmp.find_last_of('.') + 1);
		std::cout << "-> " << tmp << std::endl;
		if (!tmp.compare("css"))
			this->_headers["Content-Type"] = "text/css";
	}
}


void			Response::send_response()
{
	this->_response.clear();
	this->_response_c = NULL;
	// status line
	this->writeStatusLine();
	// Headers
	this->format_header();
	//CRLF to separate header and body (or end)
	this->_response.append("\r\n"); 
	// Message-Body
	if (!this->_bufb.empty())
	{
		this->_response.append(this->_bufb);
	}
	// if (this->_buf)
	// {

	// 	this->_response.append(&this->_buf[this->_idx]);
	// 	delete [] this->_buf;
	// }
	// this->_buf = NULL;
	// std::cout << "------------ BUF -----------------" << std::endl;
	// std::cout << this->_buf;
	// std::cout << "---------------------------------------" << std::endl;
	//print on our cout to check
	std::cout << "------------ RESPONSE -----------------" << std::endl;
	std::cout << this->_response;
	std::cout << "---------------------------------------" << std::endl;
	// Actually sending to socket
	// delete [] this->_buf;
	// this->_buf = NULL;
	send(this->_socket, this->_response.c_str(), this->_response.size(), 0);
	this->_cgi.free_buf();
}

