/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myCGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 15:26:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myCGI.hpp"
#define CGI_BUFSIZE 2000

# define PATH "/index.html"

myCGI::myCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest())
{
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["STATUS_CODE"] = "200 OK";
	this->_env["AUTH_TYPE"] = std::string();
	this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->setPathQuery();
	this->_env["REMOTE_ADDR"] = std::string();
	this->_env["REMOTE_IDENT"] = std::string();
	this->_env["REMOTE_USER"] = std::string();
	this->_env["REQUEST_METHOD"] = this->_request.getMethod();
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	//___
	// this->_env["REDIRECT_STATUS"] = "200"; 
	// this->_env["STATUS_CODE"] = std::string();
	// this->_env["AUTH_TYPE"] = std::string();
	// this->_env["CONTENT_LENGTH"] = std::string(); //not sure
	// this->_env["CONTENT_TYPE"] = std::string();
	// this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// this->_env["REMOTE_ADDR"] = std::string();
	// this->_env["REMOTE_IDENT"] = std::string();
	// this->_env["REMOTE_USER"] = std::string();
	// this->_env["REQUEST_METHOD"] = "HEAD";
	// this->_env["REQUEST_URI"] = "/index.html";
	// this->_env["SCRIPT_FILENAME"] = "index.html";
	// this->_env["SCRIPT_NAME"] = "index.html";
	// this->_env["SERVER_NAME"] = "localhost";
	// this->_env["SERVER_PORT"] = "8080";
	// this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	// this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	// this->_env["PATH_TRANSLATED"] = "/index.html";
	// this->_env["PATH_INFO"] = "/index.html";
	// this->_env["QUERY_STRING"] = std::string();
	this->execCGI();
	// std::cout << "SUCCESS" << std::endl;
	Response((*this), this->_server.getSocket());
}

myCGI::~myCGI()
{
	free(this->_buf);
}

void	myCGI::setPathQuery()
{
	std::string	target = this->_request.getTarget();
	this->_env["REQUEST_URI"] = target;
	this->_env["PATH_TRANSLATED"] = target;
	this->_env["PATH_INFO"] = target.substr(0, target.find('?'));
	this->_env["SCRIPT_FILENAME"] = target.substr(1, target.find('?'));
	this->_env["SCRIPT_NAME"] = target.substr(1, target.find('?'));
	if (this->_env["PATH_INFO"].empty())
		this->_env["PATH_INFO"] = PATH;
	if (this->_env["SCRIPT_FILENAME"].empty())
		this->_env["SCRIPT_FILENAME"] = PATH;
	if (this->_env["SCRIPT_NAME"].empty())
		this->_env["SCRIPT_NAME"] = PATH;
	this->_env["QUERY_STRING"] = std::string();
	if (target.find('?') != std::string::npos)
		this->_env["QUERY_STRING"] = target.substr(target.find('?'), target.size()); //maybe wrong if no '?'
}

std::string const	myCGI::getEnvVar(std::string var_name) const
{
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	while (it != this->_env.end())
	{
		if (var_name.compare(it->first) == 0)
			return it->second;
		it++;
	}
	return std::string();
}

std::string const 	myCGI::getBuf() const
{
	return std::string(this->_buf);
}

char	**myCGI::env_to_char_array()
{
	char	**array = new char*[_env.size() + 1];
	int		j = 0;

	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++) 
	{
		std::string	element = it->first + "=" + it->second;
		// array[j] = ft_strdup((char *)element.c_str());
		array[j] = new char[element.size() + 1];
		array[j] = strcpy(array[j], (const char*)element.c_str());
		j++;
	}
	array[j] = NULL;
	return array;
}

void	myCGI::execCGI()
{
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
	else if (!pid)
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve("/usr/bin/php-cgi", nll, env_array);
		// execve("cgi_tester", nll, env_array);
		std::cerr <<  "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			std::cout << buffer << std::endl;
			newBody += buffer;
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

	for (size_t i = 0; env_array[i]; i++)
	{
		// std::cout << env_array[i] << std::endl;
		delete[] env_array[i];
	}	
	delete[] env_array;

	if (!pid)
		exit(0);

	if (!(this->_buf = (char *)malloc(sizeof(char) * newBody.size())))
		exit(EXIT_FAILURE);
	ft_memcpy(this->_buf, (char *)newBody.c_str(), newBody.size());
}
