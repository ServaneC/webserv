/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 15:54:48 by schene            #+#    #+#             */
/*   Updated: 2021/07/28 18:07:09 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Request.hpp"


int			Request::recvBody(const std::list<Location*> &routes)
{
	this->_bad_request = 0;
	this->setMaxBodySize(routes);
	if (!this->getHeaderField("Content-Length").empty())
	{
		for (int i = 0; i < std::atoi(this->_headers["Content-Length"].c_str()); i++)
			this->recvBodyCL(1);
	}
	else if (!this->getHeaderField("Transfer-Encoding").compare("chunked"))
		recvChunk();
	else
		this->_bad_request = 411;
	return 1;
}

int			Request::recvBodyCL(int size)
{
	unsigned char *recv_buf;
	int  recv_ret;

	if (size > this->_max_body_size)
	{
		this->_bad_request = 413;
		return -1;
	}
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
			if (this->_body_size > this->_max_body_size)
			{
				this->_bad_request = 413;
				return -1;
			}
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
		if (recv_ret < 0)
			return (-1);
		if (recv_ret > 0)
			break ;
		else
		{
			this->_stop = true;
			break;
		}
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
