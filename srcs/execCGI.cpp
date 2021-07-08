/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execCGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 15:26:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execCGI.hpp"
#define _BUFSIZE 2000

// # define PATH "/home/lilou/my_home/SC_webserv/"
# define PATH "/Users/schene/Desktop/webserv/"
# define INDEX "index.html"

execCGI::execCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest()), _buf(NULL),
		 _buf_size(0), _last_modified(0), _argv(NULL)
{
	// VM
	// std::string cgi_path = "/usr/bin/php-cgi";
	// 42 MAC
	// std::string cgi_path = "/Users/schene/.brew/Cellar/php/8.0.7/bin/php-cgi";
	// OTHER MAC
	std::string cgi_path = "/usr/local/Cellar/php/8.0.7/bin/php-cgi";

	this->_env["STATUS_CODE"] = "200 OK";
	if (this->_location_list.size() > 1)
		this->_location_list.erase(this->_location_list.begin());
	this->_buf = NULL;
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	if (!this->_request.getBadRequest())
	{
		this->_env["REQUEST_METHOD"] = this->_request.getMethod();
		this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
		this->setPathQuery();
	}
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["HTTP_ACCEPT"] = this->_request.getHeaderField("Accept"); 
	if (this->_request.getBadRequest())
		this->_env["STATUS_CODE"] = "400 Bad Request";
	// this->_env["AUTH_TYPE"] = std::string();
	if (!this->_request.getHeaderField("Content-Length").empty())
		this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	// if (!this->_request.getHeaderField("Accept").empty())
	// 	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Accept");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// this->_env["REMOTE_ADDR"] = std::string();
	// this->_env["REMOTE_IDENT"] = std::string();
	// this->_env["REMOTE_USER"] = std::string();
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	
	Autoindex autoidx(this->_request.getTarget(), this->_env["PATH_INFO"]);
	if (autoidx.isDir() && !autoidx.getIndex())
	{
		std::string tmp = autoidx.autoindex_generator();
		this->_buf_size = tmp.size();
		this->_buf = new unsigned char[tmp.size()];
		for (size_t i = 0; i < tmp.size(); i++)
			this->_buf[i] = tmp[i];
	}
	else
	{
		if (autoidx.isDir() && autoidx.getIndex())
		{
			this->_env["PATH_INFO"] = this->_env["PATH_INFO"] + '/' + std::string(INDEX);
			this->_env["SCRIPT_FILENAME"] = this->_env["PATH_INFO"];
			this->_env["SCRIPT_NAME"] = this->_env["PATH_INFO"];
			this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
		}	
		try {
			this->_argv = new char*[3];
			this->_argv[0] = new char[cgi_path.size() + 1];
			std::strcpy(_argv[0], cgi_path.c_str());
			this->_argv[1] = NULL;
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl; }

		// this->_buf = NULL;
		// if (this->_env["REQUEST_METHOD"].compare("GET") == 0)
		// 	this->readFile();
		// else
			this->exec_CGI();
	}
	Response((*this), this->_server.getClientSocket());
}

execCGI::~execCGI()
{

}

void	execCGI::setPathQuery()
{
	std::string path;
	std::string	target = this->_request.getTarget();
	std::cout << "TARGET -> [" << target << "]" << std::endl;
	if (target.find('?') != std::string::npos)
	{
		this->_env["QUERY_STRING"] = target.substr(target.find('?') + 1, target.size()); //maybe wrong if no '?'
		target.erase(target.find('?'), target.size());	
	}
	std::cout << "TARGET -> [" << target << "]" << std::endl;
	try {
		path = setPathInfo(_server, _request, target);
	}
	catch (methodNotAllowedException &e) {
        this->_env["STATUS_CODE"] = e.what();
	}
	this->_env["PATH_TRANSLATED"] = path;
	this->_env["PATH_INFO"] = path;
	this->_env["REQUEST_URI"] = path;
	this->_env["SCRIPT_FILENAME"] = path;
	this->_env["SCRIPT_NAME"] = path;
}


char	**execCGI::env_to_char_array()
{
	char	**array = new char*[this->_env.size() + 1];
	int		j = 0;

	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++) 
	{
		std::string	element = it->first + "=" + it->second;
		array[j] = new char [element.size() + 1];
		array[j] = std::strcpy(array[j], element.c_str());
		j++;
	}
	array[j] = NULL;
	return array;
}

int		execCGI::set_argv()
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

void	execCGI::exec_CGI()
{
	// std::cout << "this->_env[STATUS_CODE] -> " << this->_env["STATUS_CODE"] << std::endl;
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		return ;
	
	this->_env["HTTP_HOST"] = this->_env["SERVER_NAME"];
	std::cout << "-> REQUEST_METHOD = |" << this->_env["REQUEST_METHOD"] << '|' << std::endl;
	std::cout << "-> REDIRECT_STATUS = |" << this->_env["REDIRECT_STATUS"] << '|' << std::endl;
	std::cout << "-> SCRIPT_FILENAME = |" << this->_env["SCRIPT_FILENAME"] << '|' << std::endl;
	std::cout << "-> SCRIPT_NAME = |" << this->_env["SCRIPT_NAME"] << '|' << std::endl;
	std::cout << "-> PATH_INFO = |" << this->_env["PATH_INFO"] << '|' << std::endl;
	std::cout << "-> SERVER_NAME = |" << this->_env["SERVER_NAME"] << '|' << std::endl;
	std::cout << "-> SERVER_PROTOCOL = |" << this->_env["SERVER_PROTOCOL"] << '|' << std::endl;
	std::cout << "-> REQUEST_URI = |" << this->_env["REQUEST_URI"] << '|' << std::endl;
	std::cout << "-> HTTP_HOST = |" << this->_env["HTTP_HOST"] << '|' << std::endl;
	std::cout << "-> CONTENT_LENGTH = |" << this->_env["CONTENT_LENGTH"] << '|' << std::endl;
	std::cout << "-> CONTENT_TYPE = |" << this->_env["CONTENT_TYPE"] << '|' << std::endl;
	std::cout << "-> QUERY_STRING = |" << this->_env["QUERY_STRING"] << '|' << std::endl;
	
	//just to pass second test (very ugly, should be handle differently)
	this->_buf = NULL;
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		return ;

	char		**env_array = this->env_to_char_array();
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;

	this->set_argv();

	// std::cout << "BODY SIZE =" << this->_request.getBodySize() << std::endl;
	// write(1, this->_request.getBody(), this->_request.getBodySize());
	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, this->_request.getBody(), this->_request.getBodySize());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();

	if (pid == -1) 
	{
		std::cerr << "Fork crashed." <<  std::endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) //in child 
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(this->_argv[0], this->_argv, env_array);
		std::cerr <<  "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else //in parent
	{
		unsigned char	buffer[_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, _BUFSIZE);
			ret = read(fdOut, buffer, _BUFSIZE - 1);
			buffer[ret] = 0;
			if (ret > 0)
				this->append_body(buffer, ret);
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	this->_request.reset_body();
	if (this->_argv[0])
		delete [] this->_argv[0];
	this->_argv[0] = NULL;
	if (this->_argv[1])
		delete [] this->_argv[1];
	if (this->_argv)
		delete [] this->_argv;

	this->_argv = NULL;

	for (int i = 0; env_array[i]; i++)
		delete [] env_array[i];
	delete [] env_array;

	if (!pid)
		exit(0);
}

// void	execCGI::exec_GET()
// {

// }

int		execCGI::readFile()
{
	int					fd;
	unsigned char		*buffer;
	struct stat			info;

	fd = open(this->_env["PATH_INFO"].c_str(), O_RDONLY);
	if (fd < 0)
		return (403);
	fstat(fd, &info);
	buffer = new unsigned char [info.st_size];
	if (read(fd, buffer, info.st_size) < 0)
		return(500);
	this->append_body((unsigned char *)buffer, info.st_size);
	close(fd);
	return (200);
}


void				execCGI::free_buf()
{
	delete [] this->_buf;
	this->_buf = NULL;
}

void		execCGI::append_body(unsigned char *buffer, int size)
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
std::string const	&execCGI::getEnvVar(std::string var_name) const
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

std::string const	&execCGI::getRequestHeader(std::string field_name) const
{
	return this->_request.getHeaderField(field_name);
}


unsigned  char		 *execCGI::getBuf() const
{
	return (unsigned char *)this->_buf;
}

int			execCGI::getBufSize() const
{
	return this->_buf_size;
}

time_t				execCGI::getLastModified() const
{
	return this->_last_modified;
}
