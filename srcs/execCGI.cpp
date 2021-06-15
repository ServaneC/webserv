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

# define PATH "/Users/schene/Desktop/webserv/"
# define INDEX "index.html"

execCGI::execCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest()), _last_modified(0)
{
	// VM
	std::string cgi_path = "/usr/bin/php-cgi";
	// 42 MAC
	// std::string cgi_path = "/Users/schene/.brew/Cellar/php/8.0.7/bin/php-cgi";
	// OTHER MAC
	//std::string cgi_path = "/usr/local/Cellar/php/8.0.7/bin/php-cgi";

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
	if (!(this->_argv = (char **)malloc(sizeof(char *) * 3)))
		return ;
	if (!(this->_argv[0] = (char *)malloc(sizeof(char) * cgi_path.size() + 1)))
		return ;
	std::strcpy(_argv[0], cgi_path.c_str());
	this->_argv[1] = NULL;
	this->exec_CGI();
	// std::cout << "SUCCESS" << std::endl;
	Response((*this), this->_server.getClientSocket());
}

execCGI::~execCGI()
{
	if (this->_argv[0])
	{
		free(this->_argv[0]);
		if (this->_argv[1])
			free(this->_argv[1]);
		free(this->_argv);
	}
	this->_argv = NULL;
	// if (this->_buf)
	// 	free(this->_buf);
}

void	execCGI::setPathQuery()
{
	std::string	target = this->_request.getTarget();
	if (target[0] == '/')
		target.erase(0, 1);

	std::cout << "TARGET -> [" << target << "]" << std::endl;
	this->_env["REQUEST_URI"] = target;
	this->_env["PATH_TRANSLATED"] = target;
	this->_env["PATH_INFO"] = std::string(PATH) + target.substr(0, target.find('?'));
	this->_env["SCRIPT_FILENAME"] = target.substr(0, target.find('?'));
	this->_env["SCRIPT_NAME"] = target.substr(0, target.find('?'));
	if (target.empty())
		this->_env["PATH_INFO"] += std::string(INDEX);
	if (this->_env["SCRIPT_FILENAME"].empty() || !this->_env["SCRIPT_FILENAME"].compare("/"))
		this->_env["SCRIPT_FILENAME"] = INDEX;
	if (this->_env["SCRIPT_NAME"].empty() || !this->_env["SCRIPT_NAME"].compare("/"))
		this->_env["SCRIPT_NAME"] = INDEX;
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

int		execCGI::set_argv()
{
	std::string file_path = this->_env["PATH_INFO"];

	std::cout << "=> [" << file_path << ']' <<  std::endl;
	if (this->_argv[1])
		free(this->_argv[1]);
	this->_argv[1] = NULL;
	if (!(this->_argv[1] = (char *)malloc(sizeof(char) * file_path.size() + 1)))
		return (-1);
	std::strcpy(_argv[1], file_path.c_str());
	this->_argv[2] = NULL;
	return 1;
}

void	execCGI::exec_CGI()
{
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
	std::cout << "-> QUERY_STRING = |" << this->_env["QUERY_STRING"] << '|' << std::endl;
	
	//just to pass second test (very ugly, should be handle differently)
	this->_buf = NULL;
	if (this->_request.getBadRequest())
	{
		this->_env["STATUS_CODE"] = "400 Bad Request";
		return ;
	}

	// switch (this->_request.getMethodCode())
	// {
	// 	case METHOD_NOT_ALLOWED:
	// 		this->_env["STATUS_CODE"] = "405 Method Not Allowed";
	// 		return ;
	// 	case METHOD_GET:
	// 		[...]
	// 		break;
	// 	case METHOD_POST:
	// 		[...]
	// 		break;
	// 	case METHOD_DELETE:
	// 		[...]
	// 		break;
	// }

	std::string			path_info = this->_env["PATH_INFO"];
	std::list<Location>	location_list = _server.getRelevantLocations(path_info);

	char		**env_array = this->env_to_char_array();
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string	newBody;
	//std::string _body;

	this->set_argv();


	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	//write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();

	if (pid == -1) 
	{
		std::cerr << "Fork crashed." <<  std::endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) //in child 
	{
		//char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(this->_argv[0], this->_argv, env_array);
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
	memset(this->_buf, 0, newBody.size() + 1);
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

char		 *execCGI::getBuf() const
{
	return this->_buf;
}

time_t				execCGI::getLastModified() const
{
	return this->_last_modified;
}