/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execPostMethod.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 18:25:24 by schene            #+#    #+#             */
/*   Updated: 2021/07/28 16:06:06 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execRequest.hpp"

void	        execRequest::exec_post()
{
	this->_cgi = false;
	if (!this->_env["CONTENT_TYPE"].compare(0, 19, "multipart/form-data"))
		this->upload_file();
	else
		this->_env["STATUS_CODE"] = "204 No Content";
}

std::string     execRequest::parseRequestBody()
{
	std::string boundary;
    std::string	filename;
	std::string	type;
    int		    next_n; 

	if (this->_env["CONTENT_TYPE"].find("boundary=") != std::string::npos)
	{
		boundary = this->_env["CONTENT_TYPE"].substr(this->_env["CONTENT_TYPE"].find("boundary=") + 9);
		boundary.insert(0, 2, '-');
		this->_request_buf_start = boundary.size() + 2; // erase the first boundary + CRLF
		this->_request_buf_size -= 2 * boundary.size() + 6; // erase the last boundary + '--' + CRLF
	}
	next_n = ft_gnl(this->_request.getBody(), this->_request_buf_start);
	filename = std::string((char *)(this->_request.getBody() + this->_request_buf_start), next_n - 1);
    this->_request_buf_start += next_n + 1;
	this->_request_buf_size -= next_n + 1;
	next_n = ft_gnl(this->_request.getBody(), this->_request_buf_start);
	type = std::string((char *)(this->_request.getBody() + this->_request_buf_start), next_n - 1);
	this->_request_buf_start += next_n + 3;
	this->_request_buf_size -= next_n + 3;
	filename.erase(0, filename.find("filename=\""));
	filename.erase(0, filename.find('\"') + 1);
	filename.erase(filename.find_last_of('\"'), 1);
	this->_env["CONTENT_TYPE"] = type.substr(type.find(':') + 2);
	filename.insert(0, this->_upload_path + '/');
    return (filename);
}

void	        execRequest::upload_file()
{
	struct stat info;
	std::string body;
    std::string filename = this->parseRequestBody();

	if (this->_upload_path.empty())
	{
		body = "<!DOCTYPE HTML>\n<html><body><h1> There is no upload_path in conf for this Location !</h1>\n";
		body += "Please configure one and then you can try again ! </body>\n</html>\n";
		this->append_body((unsigned char *)body.c_str(), body.size());
		return ;
	}
	if (lstat(filename.c_str(), &info) == 0)
		body = "<!DOCTYPE HTML>\n<html><body><h1> This file was already uploaded!</h1>\n";
	else
	{
		int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 00755);
		if (fd < 0)
		{
			body = "<!DOCTYPE HTML>\n<html><body><h1> ERROR while opening/creating file !</h1>\n";
			this->append_body((unsigned char *)body.c_str(), body.size());
			return ;
		}
		write(fd, &(this->_request.getBody()[this->_request_buf_start]), this->_request_buf_size);
		close(fd);
		this->_env["LOCATION"] = '/' + filename;
		this->_env["STATUS_CODE"] = "201 Created";

		body = "<!DOCTYPE HTML>\n<html><body><h1> File Uploaded !</h1>\n";
	}
	body += "<p>Go <a href = \"" + filename + "\">" +  "here</a> to check it out</p>\n";
	body += "</body>\n</html>\n";
	this->append_body((unsigned char *)body.c_str(), body.size());
}