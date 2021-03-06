/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/07/18 17:29:14 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execRequest.hpp"
#define _BUFSIZE 2000

execRequest::execRequest(Server &serv) 
	: _server(serv), _request(serv.getRequest()), _error_code(200), _request_buf_start(0), 
		_request_buf_size(_request.getBodySize()), _buf(NULL),
		_buf_size(0), _last_modified(0), _argv(NULL), _cgi(false)
{
	bool autoindex = false;

	if (this->_location_list.size() > 1)
		this->_location_list.erase(this->_location_list.begin());
	this->_buf = NULL;
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["REQUEST_URI"] = _request.getTarget();
	this->_env["HTTP_ACCEPT"] = this->_request.getHeaderField("Accept"); 
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	this->_error_pages = getRelevantLocation(_server.getRoutes(), this->_request.getTarget()).getErrorPage();
	this->_env["STATUS_CODE"] = "200 OK";
	
	if (this->_request_buf_size > 0)
		this->_env["CONTENT_LENGTH"] = ft_itoa_cpp(this->_request_buf_size);
	
	if (!this->_request.getBadRequest())
	{
		this->_env["REDIRECT_STATUS"] = "200"; 
		this->_env["REQUEST_METHOD"] = this->_request.getMethod();
		this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
		autoindex = this->setPathQuery();
	}
	else
		_error_code = this->_request.getBadRequest();
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		this->_error_code = std::atoi(this->_env["STATUS_CODE"].substr(0, 3).c_str());
	this->setStatusCode();
	if (autoindex == false)
		this->exec_method();
	Response((*this), this->_server.getClientSocket());
}

execRequest::~execRequest() 
{
	if (this->_argv && this->_argv[0])
	{
		delete [] this->_argv[0];
		if (this->_argv[1])
			delete [] this->_argv[1];
		delete [] this->_argv;
	}	
}

bool execRequest::tryPath(Server &server, Request &request, const std::string &target)
{
	const Location  &loc = getRelevantLocation(server.getRoutes(), target);
	std::string		cgi_path;

	this->_error_pages = loc.getErrorPage();
	if (loc.getCGIPath().size())
		cgi_path = loc.getCGIPath();
	
	if (loc.getUploadPath().size())
		this->_upload_path = loc.getUploadPath();

    if (chdir(request.getDirPath().c_str()) == -1)
	{
		if (errno ==  EACCES)
			throw ForbiddenException();
		throw chdirFailException();
	}

	this->setPathName(this->_request.getDirPath(), this->_request.getObject());
	
	if (!loc.getRedirectURL().empty())
	{
		this->_error_code = 301;
		this->_env["STATUS_CODE"] = "301 Moved Permanently";
		this->_env["LOCATION"] = loc.getRedirectURL();
		return false;
	}
    Autoindex autoidx(target, this->_env["PATH_INFO"], loc.getIndexes());
	this->_last_modified = autoidx.getLastModified();
	if (!autoidx.path_exist())
	{
		this->_error_code = 404;
		return false;
	}
	else if (autoidx.isDir() && !autoidx.getIndex() && !this->_env["REQUEST_METHOD"].compare("GET"))
	{
		if (!loc.getAutoIndex())
		{
			this->_error_code = 403;
			return false;
		}
		else
		{
			std::string tmp = autoidx.autoindex_generator();
			this->_buf_size = tmp.size();
			this->_buf = new unsigned char[tmp.size()];
			for (size_t i = 0; i < tmp.size(); i++)
				this->_buf[i] = tmp[i];
			return true;
		}	
	}
	else
	{
		if (autoidx.isDir() && !this->_env["REQUEST_METHOD"].compare("GET"))  // is a dir and index present
		{
			const std::string index = firstValidIndex(loc.getIndexes());
			this->setPathName(this->_env["PATH_INFO"], index);
		}
		try {
			if (cgi_path.size())
			{
				this->_cgi = true;
				this->_argv = new char*[3];
				this->_argv[0] = new char[cgi_path.size() + 1];
				std::strcpy(_argv[0], cgi_path.c_str());
				this->_argv[1] = NULL;
			}
		}
		catch (std::exception &e) {
			std::cout << "In TryPath: " << e.what() << std::endl; }
		return false;
	}
}

void	execRequest::setPathName(std::string path_info, std::string object)
{
	this->_env["PATH_INFO"] = path_info + '/' + object;
	this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
	this->_env["SCRIPT_FILENAME"] = object;
	this->_env["SCRIPT_NAME"] = object;
}

bool	execRequest::setPathQuery()
{
	std::string path;
	std::string	target = this->_request.getTarget();
	std::string object = this->_request.getObject();
	bool autoindex = false;

	if (target.find('?') != std::string::npos)
	{
		this->_env["QUERY_STRING"] = target.substr(target.find('?') + 1, target.size());
		target.erase(target.find('?'), target.size());
		object.erase(object.find('?'), object.size());
		this->_request.setObject(object);
	}	
	if (object.find_last_of('.') != std::string::npos)
		this->_file_ext = object.substr(object.find_last_of('.') + 1);
	try {
		this->_request.setDirPath(buildPath(_server, _request, target));
		autoindex = tryPath(_server, _request, target);
	}
	catch (std::exception &e) {
        this->_env["STATUS_CODE"] = e.what();
	}
	return (autoindex);
}


int		execRequest::set_argv()
{
	if (this->_argv[1])
		delete [] this->_argv[1];
	this->_argv[1] = NULL;
	this->_argv[1] = new char[this->_env["PATH_INFO"].size() + 1];
	std::strcpy(_argv[1], this->_env["PATH_INFO"].c_str());
	this->_argv[2] = NULL;
	return 1;
}

void				execRequest::free_buf()
{
	delete [] this->_buf;
	this->_buf = NULL;
}

void		execRequest::append_body(unsigned char *buffer, int size)
{
	try {
		if (this->_buf)
		{
			this->_buf = MyRealloc(this->_buf, this->_buf_size, this->_buf_size + size + 1);
			std::memmove(this->_buf + this->_buf_size, buffer, size);
		}
		else
		{
			this->_buf = new unsigned char[size + 1];
			std::memmove(this->_buf, buffer, size);
		}
		this->_buf_size += size;
	}
	catch (std::exception &e) {
        std::cout << e.what() << std::endl; }
}


// GETTERS
std::string const	&execRequest::getEnvVar(std::string var_name) const
{
	static std::string const	empty;
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	while (it != this->_env.end())
	{
		if (var_name.compare(it->first) == 0)
			return it->second;
		it++;
	}
	return empty;
}

std::string const	&execRequest::getRequestHeader(std::string field_name) const
{
	return this->_request.getHeaderField(field_name);
}


unsigned  char		 *execRequest::getBuf() const
{
	return (unsigned char *)this->_buf;
}

int			execRequest::getBufSize() const
{
	return this->_buf_size;
}

time_t				execRequest::getLastModified() const
{
	return this->_last_modified;
}

bool				execRequest::getCgi() const
{
	return this->_cgi;
}
