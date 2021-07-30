/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 18:11:49 by schene            #+#    #+#             */
/*   Updated: 2021/07/30 16:04:08 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execRequest.hpp"

void	execRequest::exec_method()
{
	if (this->_error_code == 200)
	{
		if (!this->_env["REQUEST_METHOD"].compare("DELETE"))
			this->exec_delete();
		else if (!this->_file_ext.compare("php"))
		{
			if (this->_cgi)
				this->exec_CGI();
			else
			{
				this->_error_code = 500;
				std::string no_cgi = "ERROR: No path to php-cgi in conf file\n";
				this->append_body((unsigned char *)no_cgi.c_str(), no_cgi.size());
			}
		}
		else if (!this->_env["REQUEST_METHOD"].compare("POST"))
			this->exec_post();
		else if (!this->_env["REQUEST_METHOD"].compare("GET"))
			this->readFile();
	}
	if (this->_error_code != 200)
	{
		this->setStatusCode();
		this->serveErrorPage();	
	}
}

void		execRequest::readFile()
{
	int					fd;
	unsigned char		*buffer;
	struct stat			info;

	this->_cgi = false;
	fd = open(this->_env["PATH_INFO"].c_str(), O_RDONLY);
	if (fd < 0)
		this->_error_code = 500;
	else
	{
		fstat(fd, &info);
		buffer = new unsigned char [info.st_size];
		if (read(fd, buffer, info.st_size) < 0)
			this->_error_code = 500;
		else
			this->append_body((unsigned char *)buffer, info.st_size);
		delete [] buffer;
		close(fd);
	}
}

void	execRequest::exec_delete()
{

	if (remove(this->_env["PATH_INFO"].c_str()) < 0)
		this->_error_code = 403;
	else
		this->_env["STATUS_CODE"] = "204 No Content";
}

void execRequest::serveErrorPage()
{
	int					fd;
	std::string			path;
	unsigned char		*buffer;
	struct stat			info;

	std::cout << "error = " << this->_error_code << std::endl;
    if (this->_error_pages.find(this->_error_code) != this->_error_pages.end())
        path = this->_error_pages.find(this->_error_code)->second;
	if (path.empty())
		return ;
	fd = open(path.c_str(), O_RDONLY);
	if (fstat(fd, &info) == 0)
	{
		buffer = new unsigned char [info.st_size];
		if (read(fd, buffer, info.st_size) < 0)
			this->_env["STATUS_CODE"] = "500 Internal Server Error";
		else
			this->append_body((unsigned char *)buffer, info.st_size);
		delete [] buffer;
	}
	close(fd);
}

void	execRequest::setStatusCode()
{
	if (this->_error_code == 100)
		this->_env["STATUS_CODE"] = "100 Continue";
	if (this->_error_code == 400)
		this->_env["STATUS_CODE"] = "400 Bad Request";
	if (this->_error_code == 403)
		this->_env["STATUS_CODE"] = "403 Forbidden";
	if (this->_error_code == 404)
		this->_env["STATUS_CODE"] = "404 Not Found";
	if (this->_error_code == 405)
		this->_env["STATUS_CODE"] = "405 Method Not Allowed";
	if (this->_error_code == 411)
		this->_env["STATUS_CODE"] = "411 Length Required";
	if (this->_error_code == 413)
		this->_env["STATUS_CODE"] = "413 Request Entity Too Large";
	if (this->_error_code == 500)
		this->_env["STATUS_CODE"] = "500 nternal Server Error";	
}
