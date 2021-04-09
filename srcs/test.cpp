/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:14:33 by schene            #+#    #+#             */
/*   Updated: 2021/04/09 10:23:58 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../libft/include/libft.h"

#define CGI_BUFSIZE 2000
#include "../webserv.hpp"

void		setenv(std::map<std::string, std::string> &env)
{
	env["REDIRECT_STATUS"] = "200"; 
	env["STATUS_CODE"] = std::string();
	env["AUTH_TYPE"] = std::string();
	env["CONTENT_LENGTH"] = std::string(); //not sure
	env["CONTENT_TYPE"] = std::string();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["REMOTE_ADDR"] = std::string();
	env["REMOTE_IDENT"] = std::string();
	env["REMOTE_USER"] = std::string();
	env["REQUEST_METHOD"] = "GET";
	env["REQUEST_URI"] = "/index.html";
	env["SCRIPT_FILENAME"] = "index.html";
	env["SCRIPT_NAME"] = "index.html";
	env["SERVER_NAME"] = "localhost";
	env["SERVER_PORT"] = "8080";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["PATH_TRANSLATED"] = "/media/sf_Desktop/webserv/srcs/index.html";
	env["PATH_INFO"] = "/index.html";
	env["QUERY_STRING"] = std::string();
	
}

char					**getEnvAsCstrArray(std::map<std::string, std::string> &_env)
{
	char	**env = new char*[_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = _env.begin(); i != _env.end(); i++) 
	{
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}


int main(void) 
{
	
	std::map<std::string, std::string>	env;
	char								**env_array;
	
	setenv(env);
	env_array = getEnvAsCstrArray(env);

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
		return -1;
		// return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		// char *arg[3];
		// arg[0] = (char *)("../cgi_tester");
		// arg[1] = (char *)("index.html");
		// arg[2] = NULL;	
		execve("../ubuntu_cgi_tester", nll, env_array);
		// execve("/usr/bin/php-cgi", nll, env_array);
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

	// std::cout << newBody << std::endl;
	return 1;
	// return (newBody);
}

// {
// 	std::map<std::string, std::string>	env;
// 	char								**env_array;
	
// 	setenv(env);
// 	env_array = getEnvAsCstrArray(env);
// 	pid_t pid;
// 	int exec_res;
// 	char **exec_args;
// 	int tmp_fd;
// 	int fd[2];

// 	// Log("Call CGI.");
// 	// exec_args = _getExecArgs();
// 	if (pipe(fd) == -1)
// 	{
// 		perror ("In pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 		// throw CGIException("Cannot create pip to execute CGI.");
// 	pid = fork();
// 	if (pid == 0) //in child
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], 0);
// 		tmp_fd = open("webserv_cgi", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
// 		if (tmp_fd < 0)
// 		{
// 			perror ("fail open");
// 			exit(EXIT_FAILURE);
// 		}
// 			// throw CGIException("Cannot create temporary file to catch CGI output in /tmp.");
// 		dup2(tmp_fd, 1);
// 		dup2(tmp_fd, 2);
// 		char * const * nll = NULL;
// 		exec_res = execve("../cgi_tester", nll, env_array);
// 		std::cout << "exec = " << exec_res << std::endl;
// 		// exec_res = execve(_cgi_path.c_str(), exec_args, args);
// 		close(0);
// 		close(tmp_fd);
// 		close(fd[0]);
// 		exit(0);
// 	}
// 	else // in parent
// 	{
// 		close(fd[0]);
// 		// write(fd[1], _request.getContent().c_str(), _request.getContent().length());
// 		close(fd[1]);
// 		waitpid(-1, NULL, 0);
// 		// _freeArgs(args);
// 		// _freeArgs(exec_args);
// 	}
// 	// Log("End CGI");
// 	// return (readFile("/tmp/webserv_cgi"));
// }