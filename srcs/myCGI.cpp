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

#include "include/myCGI.hpp"
#define CGI_BUFSIZE 2000

# define PATH "/index.html"

myCGI::myCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest())
{
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["STATUS_CODE"] = "200 OK";
	// this->_env["AUTH_TYPE"] = std::string();
	this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->setPathQuery();
	// this->_env["REMOTE_ADDR"] = std::string();
	// this->_env["REMOTE_IDENT"] = std::string();
	// this->_env["REMOTE_USER"] = std::string();
	this->_env["REQUEST_METHOD"] = this->_request.getMethod();
	this->_env["SERVER_NAME"] = this->_server.getName();
	this->_env["SERVER_PORT"] = this->_server.getPort();
	this->_env["SERVER_PROTOCOL"] = this->_request.getHTTPVersion();
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
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
	if (this->_env["PATH_INFO"].empty() || this->_env["PATH_INFO"].compare("/"))
		this->_env["PATH_INFO"] = PATH;
	if (this->_env["SCRIPT_FILENAME"].empty() || this->_env["SCRIPT_FILENAME"].compare("/"))
		this->_env["SCRIPT_FILENAME"] = "index.html";
	if (this->_env["SCRIPT_NAME"].empty() || this->_env["SCRIPT_NAME"].compare("/"))
		this->_env["SCRIPT_NAME"] = "index.html";
	// this->_env["QUERY_STRING"] = std::string();
	if (target.find('?') != std::string::npos)
		this->_env["QUERY_STRING"] = target.substr(target.find('?'), target.size()); //maybe wrong if no '?'
}

std::string const	&myCGI::getEnvVar(std::string var_name) const
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

std::string const 	&myCGI::getBuf() const
{
	static std::string const	ret = this->_buf;
	return ret;
}

char	**myCGI::env_to_char_array()
{
	char	**array;
	int		j = 0;

	// array = new char*[this->_env.size() + 1];
	if (!(array = (char **)malloc(sizeof(char *) * (this->_env.size() + 1))))
		exit(EXIT_FAILURE);

	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++) 
	{
		std::string	element = it->first + "=" + it->second;
		array[j] = ft_strdup((char *)element.c_str()); //cast bc c_str() = const char *
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

	// for (int i = 0; env_array[i]; i++)
	// 	free(env_array[i]);
	// free(env_array);

	if (!pid)
		exit(0);

	if (!(this->_buf = (char *)malloc(sizeof(char) * (newBody.size() + 1))))
		exit(EXIT_FAILURE);
	ft_memcpy(this->_buf, (char *)newBody.c_str(), newBody.size());
	this->_buf[newBody.size()] = 0;
}
