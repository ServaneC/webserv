/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/07/19 17:56:38 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execRequest.hpp"
#define _BUFSIZE 2000

char	**execRequest::env_to_char_array()
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

void	execRequest::exec_CGI()
{
	std::cout << "CALL TO THE CGI !" << std::endl;
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		return ;
	
	this->_env["HTTP_HOST"] = this->_env["SERVER_NAME"];

	this->_buf = NULL;
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		return ;

	char		**env_array = this->env_to_char_array();
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;

	this->set_argv();
	
	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, &(this->_request.getBody()[this->_request_buf_start]), this->_request_buf_size);
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
