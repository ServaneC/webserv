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
#define CGI_BUFSIZE 2000

# define PATH "/index.html"

execCGI::execCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest())
{
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	//std::cout << "CGI -> " << this->_request.getBadRequest() << std::endl;
	if (!this->_request.getBadRequest())
	{
		this->_env["REQUEST_METHOD"] = this->_request.getMethod();
		this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
		this->setPathQuery();
	}
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["STATUS_CODE"] = "200 OK";
	// this->_env["AUTH_TYPE"] = std::string();
	this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// this->_env["REMOTE_ADDR"] = std::string();
	// this->_env["REMOTE_IDENT"] = std::string();
	// this->_env["REMOTE_USER"] = std::string();
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	this->exec_CGI();
	// std::cout << "SUCCESS" << std::endl;
	Response((*this), this->_server.getSocket());
}

execCGI::~execCGI()
{
	// if (this->_buf)
	// 	free(this->_buf);
}

void	execCGI::setPathQuery()
{
	std::string	target = this->_request.getTarget();

	this->_env["REQUEST_URI"] = target;
	this->_env["PATH_TRANSLATED"] = target;
	// this->_env["PATH_INFO"] = target;
	// if (target.find('?') != std::string::npos)
		this->_env["PATH_INFO"] = target.substr(0, target.find('?'));
	this->_env["SCRIPT_FILENAME"] = target.substr(1, target.find('?'));
	this->_env["SCRIPT_NAME"] = target.substr(1, target.find('?'));
	if (this->_env["PATH_INFO"].empty() || !this->_env["PATH_INFO"].compare("/"))
		this->_env["PATH_INFO"] = PATH;
	if (this->_env["SCRIPT_FILENAME"].empty() || !this->_env["SCRIPT_FILENAME"].compare("/"))
		this->_env["SCRIPT_FILENAME"] = "index.html";
	if (this->_env["SCRIPT_NAME"].empty() || !this->_env["SCRIPT_NAME"].compare("/"))
		this->_env["SCRIPT_NAME"] = "index.html";
	// this->_env["QUERY_STRING"] = std::string();
	if (target.find('?') != std::string::npos)
		this->_env["QUERY_STRING"] = target.substr(target.find('?'), target.size()); //maybe wrong if no '?'
}

char	**execCGI::env_to_char_array()
{
	char	**array;
	int		j = 0;

	if (!(array = (char **)malloc(sizeof(char *) * (this->_env.size() + 1))))
		exit(EXIT_FAILURE);

	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++) 
	{
		std::string	element = it->first + "=" + it->second;
		if (!(array[j] = (char *)malloc(sizeof(char) * element.size() + 1)))
			return NULL;
		array[j] = strcpy(array[j], element.c_str());
		j++;
	}
	array[j] = NULL;
	return array;
}

void		execCGI::execGET()
{
	struct stat info;

	//we remove the '/' at the beginning of the path
	std::string 	path = this->_env["PATH_INFO"].substr(1, this->_env["PATH_INFO"].size());
	int fd = open(path.c_str() , O_RDONLY);
	if (fd == -1)
	{
		if (errno == 2)
		{
			this->_env["STATUS_CODE"] = "404 Not Found";
			return ;
		}
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	fstat(fd, &info);
	int size = info.st_size;
	this->_last_modified = info.st_mtime;
	std::cout << "size = " << size << std::endl;
	if (!(this->_buf = (char *)malloc(sizeof(char) * (size + 1))))
		exit(EXIT_FAILURE);
	if (read(fd, this->_buf, size) < 0)
	{
		perror("read failed");
		exit(EXIT_FAILURE);
	}
	this->_buf[size] = 0;
	close(fd);
}

void	execCGI::exec_CGI()
{
	std::cout << "==== method |" << this->_env["REQUEST_METHOD"] << '|' << std::endl;
	//just to pass second test (very ugly, should be handle differently)
	this->_buf = NULL;
	if (this->_request.getBadRequest())
	{
		this->_env["STATUS_CODE"] = "400 Bad Request";
		return ;
	}		

	if (this->_env["REQUEST_METHOD"] == "GET")
		return this->execGET();

	if (this->_env["REQUEST_METHOD"] == "POST" || this->_env["REQUEST_METHOD"] == "HEAD")
	{
		this->_env["STATUS_CODE"] = "405 Method Not Allowed";
		return ;
	}

	char		**env_array = this->env_to_char_array();
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string	newBody;
	std::string _body;

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();

	if (pid == -1) 
	{
		std::cerr << "Fork crashed." <<  std::endl;
		exit(EXIT_FAILURE);
		// return -1;
		// return ("Status: 500\r\n\r\n");
	}
	else if (pid == 0) //in child 
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve("/usr/bin/php-cgi", nll, env_array);
		// execve("cgi_tester", nll, env_array);
		std::cerr <<  "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else //in parent
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			buffer[ret] = 0;
			// std::cout << buffer << std::endl;
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut); //not sure that's allowed
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (int i = 0; env_array[i]; i++)
		free(env_array[i]);
	free(env_array);

	if (!pid)
		exit(0);

	if (!(this->_buf = (char *)malloc(sizeof(char) * (newBody.size() + 1))))
		exit(EXIT_FAILURE);
	memcpy(this->_buf, (char *)newBody.c_str(), newBody.size());
	this->_buf[newBody.size()] = 0;
}

void				execCGI::free_buf()
{
	free(this->_buf);
	this->_buf = NULL;
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

std::string const 	&execCGI::getBuf() const
{
	if (!this->_buf)
	{
		static std::string const	empty;
		return empty;
	}
	static std::string const	ret = this->_buf;
	return ret;
}

time_t				execCGI::getLastModified() const
{
	return this->_last_modified;
}