/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 18:11:49 by schene            #+#    #+#             */
/*   Updated: 2021/07/25 14:55:35 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execRequest.hpp"

void	execRequest::exec_method()
{
	if (!this->_env["REQUEST_METHOD"].compare("DELETE"))
		this->exec_delete();
	else if (!this->_env["REQUEST_METHOD"].compare("POST"))
		this->exec_post();
	else if (this->_cgi)
		this->exec_CGI();
	else if (!this->_env["REQUEST_METHOD"].compare("GET"))
		this->readFile();
}

void		execRequest::readFile()
{
	int					fd;
	unsigned char		*buffer;
	struct stat			info;

	fd = open(this->_env["PATH_INFO"].c_str(), O_RDONLY);
	if (fd < 0)
		this->_env["STATUS_CODE"] = "403 Forbidden";
	else
	{
		fstat(fd, &info);
		buffer = new unsigned char [info.st_size];
		if (read(fd, buffer, info.st_size) < 0)
			this->_env["STATUS_CODE"] = "500 Internal Server Error";
		else
			this->append_body((unsigned char *)buffer, info.st_size);
		close(fd);
	}
}

void	execRequest::exec_delete()
{

	if (remove(this->_env["PATH_INFO"].c_str()) < 0)
		this->_env["STATUS_CODE"] = "403 Forbidden";
	else
		this->_env["STATUS_CODE"] = "204 No Content";
		// this->_env["STATUS_CODE"] = "200 OK";
}