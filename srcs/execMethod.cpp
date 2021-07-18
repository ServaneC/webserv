/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/07/18 17:29:14 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execMethod.hpp"
#define _BUFSIZE 2000

execMethod::execMethod(Server &serv) 
	: _server(serv), _request(serv.getRequest()), _request_buf_start(0), 
		_request_buf_size(_request.getBodySize()), _buf(NULL),
		_buf_size(0), _last_modified(0), _argv(NULL), _cgi(false)
{
	bool autoindex = false;

	this->_env["STATUS_CODE"] = "200 OK";
	if (this->_location_list.size() > 1)
		this->_location_list.erase(this->_location_list.begin());
	this->_buf = NULL;
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["REQUEST_URI"] = _request.getTarget();
	if (!this->_request.getBadRequest())
	{
		this->_env["REQUEST_METHOD"] = this->_request.getMethod();
		this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
		autoindex = this->setPathQuery();
		printEnv("constructeur execMethod : after setPathQuery");
	}
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["HTTP_ACCEPT"] = this->_request.getHeaderField("Accept"); 
	if (this->_request.getBadRequest())
		this->_env["STATUS_CODE"] = "400 Bad Request";
	if (!this->_request.getHeaderField("Content-Length").empty())
		this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	
	if (autoindex == false && this->_env["STATUS_CODE"].compare("200 OK") == 0)
		this->exec_method();
	Response((*this), this->_server.getClientSocket());
}

execMethod::~execMethod()
{

}

bool execMethod::tryPath(Server &server, Request &request, const std::string &target)
{
	const Location  &loc = getRelevantLocation(server.getRoutes(), target);
	const Location  &ext = getRelevantExtension(server.getRoutes(), target);
	std::string		cgi_path;

	if (loc.getCGIPath().size())
		cgi_path = loc.getCGIPath();
	else if (ext.getCGIPath().size())
		cgi_path = ext.getCGIPath();

	// std::cout << "Je chdir -> " << request.getDirPath() << std::endl;
    if (chdir(request.getDirPath().c_str()) == -1)
        throw chdirFailException();

	std::cout << "DIR [" << this->_request.getDirPath() << ']' << std::endl;
	this->_env["PATH_INFO"] = this->_request.getDirPath();
	std::cout << "obj => (" << this->_request.getObject() << ')' << std::endl;
	if (!this->_request.getObject().empty())
		this->_env["PATH_INFO"] += '/' + this->_request.getObject();
	this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
	this->_env["SCRIPT_FILENAME"] = this->_request.getObject();
	this->_env["SCRIPT_NAME"] = this->_request.getObject();

    Autoindex autoidx(target, this->_env["PATH_INFO"], loc.getIndexes());
	if (!autoidx.path_exist())
		throw targetNotFoundException();
	else if (autoidx.isDir() && !autoidx.getIndex() && !this->_env["REQUEST_METHOD"].compare("GET"))
	{
		if (!loc.getAutoIndex())
			throw targetNotFoundException();
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
			this->_env["PATH_INFO"] = this->_env["PATH_INFO"] + '/' + index;
			this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
			this->_env["SCRIPT_FILENAME"] = index;
			this->_env["SCRIPT_NAME"] = index;
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
			std::cout << e.what() << std::endl; }
		return false;
	}
}

bool	execMethod::setPathQuery()
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
	std::cout << "SetPathQuery : TARGET -> [" << target << "]" << std::endl;
	std::cout << "SetPathQuery : OBJECT -> [" << object << "]" << std::endl;

	try {
		this->_request.setDirPath(buildPath(_server, _request, target));
		autoindex = tryPath(_server, _request, target);
	}
	catch (std::exception &e) {
        this->_env["STATUS_CODE"] = e.what();
	}
	return (autoindex);
}


int		execMethod::set_argv()
{
	std::string file_path = this->_env["PATH_INFO"];

	std::cout << "=> [" << file_path << ']' <<  std::endl;
	if (this->_argv[1])
		delete [] this->_argv[1];
	this->_argv[1] = NULL;
	this->_argv[1] = new char[file_path.size() + 1];
	std::strcpy(_argv[1], file_path.c_str());
	this->_argv[2] = NULL;
	return 1;
}

void	execMethod::exec_method()
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

void	execMethod::exec_post()
{
	std::string boundary;

	if (!this->_env["CONTENT_TYPE"].compare(0, 19, "multipart/form-data"))
	{
		boundary = this->_env["CONTENT_TYPE"].substr(this->_env["CONTENT_TYPE"].find("boundary=") + 9);
		boundary.insert(0, 2, '-');
		this->_request_buf_start = boundary.size() + 2; // erase the first boundary + CRLF
		this->_request_buf_size -= 2 * boundary.size() + 6; // erase the last boundary + '--' + CRLF
		this->parse_and_upload();
	}
	// printEnv("before upload");
	else if (this->_cgi)
		this->exec_CGI();
	else
		this->_env["STATUS_CODE"] = "204 No Content";
}

void	execMethod::parse_and_upload()
{
	std::string	filename;
	std::string	type;

	//PARSING
	int		next_n = ft_gnl(this->_request.getBody(), this->_request_buf_start);
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
	filename.insert(0, this->_env["SCRIPT_NAME"] + '/');

	//UPLOAD 
	struct stat info;
	std::string body;
	if (lstat(filename.c_str(), &info) == 0)
	{
		// this->_env["STATUS_CODE"] = "200 OK";
		body = "<!DOCTYPE HTML>\n<html><body><h1> File was already uploaded!</h1>\n";
	}
	else
	{
		int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 00755);
		if (fd < 0)
		{
			std::cerr << "ERROR OPEN << std::endl";
			return ;
		}
		write(fd, &(this->_request.getBody()[this->_request_buf_start]), this->_request_buf_size);
		close(fd);
		this->_env["LOCATION"] = '/' + filename;
		this->_env["STATUS_CODE"] = "201 Created";

		//BODY
		body = "<!DOCTYPE HTML>\n<html><body><h1> File Uploaded !</h1>\n";
	}
	body += "<p>Go <a href = \"" + filename + "\">" +  "here</a> to check it out</p>\n";
	body += "</body>\n</html>\n";
	this->append_body((unsigned char *)body.c_str(), body.size());
}

void		execMethod::readFile()
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

void	execMethod::exec_delete()
{

	if (remove(this->_env["PATH_INFO"].c_str()) < 0)
		this->_env["STATUS_CODE"] = "403 Forbidden";
	else
		this->_env["STATUS_CODE"] = "200 OK";
}


void				execMethod::free_buf()
{
	delete [] this->_buf;
	this->_buf = NULL;
}

void		execMethod::append_body(unsigned char *buffer, int size)
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
std::string const	&execMethod::getEnvVar(std::string var_name) const
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

std::string const	&execMethod::getRequestHeader(std::string field_name) const
{
	return this->_request.getHeaderField(field_name);
}


unsigned  char		 *execMethod::getBuf() const
{
	return (unsigned char *)this->_buf;
}

int			execMethod::getBufSize() const
{
	return this->_buf_size;
}

time_t				execMethod::getLastModified() const
{
	return this->_last_modified;
}

bool				execMethod::getCgi() const
{
	return this->_cgi;
}

void  execMethod::printEnv(std::string details)
{
	std::cout << " *** CGI ENVIRONEMENT ***\t\t" << details << std::endl;

	// std::cout << "\t{ SERVER VARIABLES }" << std::endl;
	// std::cout << "\t\t-> SERVER_SOFTWARE -> |" << this->_env["SERVER_SOFTWARE"] << "|" << std::endl;
	// std::cout << "\t\t-> SERVER_NAME -> |" << this->_env["SERVER_NAME"] << "|" << std::endl;
	// std::cout << "\t\t-> GATEWAY_INTERFACE -> |" << this->_env["GATEWAY_INTERFACE"] << "|" << std::endl;
	// std::cout << "\t\t-> HTTP_HOST = |" << this->_env["HTTP_HOST"] << '|' << std::endl;

	std::cout << "\t{ REQUEST-SPECIFIC VARIABLES }" << std::endl;
	// std::cout << "\t\t-> SERVER_PROTOCOL = |" << this->_env["SERVER_PROTOCOL"] << '|' << std::endl;
	std::cout << "\t\t-> SERVER_PORT = |" << this->_env["SERVER_PORT"] << '|' << std::endl;
	std::cout << "\t\t-> REQUEST_METHOD = |" << this->_env["REQUEST_METHOD"] << '|' << std::endl;
	std::cout << "\t\t-> REQUEST_URI = |" << this->_env["REQUEST_URI"] << '|' << std::endl;
	std::cout << "\t\t-> PATH_INFO = |" << this->_env["PATH_INFO"] << '|' << std::endl;
	std::cout << "\t\t-> PATH_TRANSLATED = |" << this->_env["PATH_TRANSLATED"] << '|' << std::endl;
	std::cout << "\t\t-> SCRIPT_NAME = |" << this->_env["SCRIPT_NAME"] << '|' << std::endl;
	std::cout << "\t\t-> SCRIPT_FILENAME = |" << this->_env["SCRIPT_FILENAME"] << '|' << std::endl;
	// std::cout << "\t\t-> QUERY_STRING -> |" << this->_env["QUERY_STRING"] << "|" << std::endl;
	// std::cout << "\t\t-> REMOTE_HOST -> |" << this->_env["REMOTE_HOST"] << "|" << std::endl;
	// std::cout << "\t\t-> REMOTE_ADDR -> |" << this->_env["REMOTE_ADDR"] << "|" << std::endl;
	// std::cout << "\t\t-> AUTH_TYPE -> |" << this->_env["AUTH_TYPE"] << "|" << std::endl;
	// std::cout << "\t\t-> CONTENT_TYPE = |" << this->_env["CONTENT_TYPE"] << '|' << std::endl;
	// std::cout << "\t\t-> CONTENT_LENGTH = |" << this->_env["CONTENT_LENGTH"] << '|' << std::endl;
	
	std::cout << "\t{ RESPONSE-SPECIFIC VARIABLES }" << std::endl;
	std::cout << "\t\t-> STATUS_CODE -> |" << this->_env["STATUS_CODE"] << "|" << std::endl;
	std::cout << "\t\t-> REDIRECT_STATUS = |" << this->_env["REDIRECT_STATUS"] << '|' << std::endl;
}
