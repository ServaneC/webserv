/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 13:24:55 by schene            #+#    #+#             */
/*   Updated: 2021/07/09 15:53:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execCGI.hpp"
#define _BUFSIZE 2000

execCGI::execCGI(Server &serv) 
	: _server(serv), _request(serv.getRequest()), _buf(NULL),
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
		printEnv("constructeur execCGI : after setPathQuery");
	}
	this->_env["REDIRECT_STATUS"] = "200"; 
	this->_env["HTTP_ACCEPT"] = this->_request.getHeaderField("Accept"); 
	if (this->_request.getBadRequest())
		this->_env["STATUS_CODE"] = "400 Bad Request";
	if (!this->_request.getHeaderField("Content-Length").empty())
		this->_env["CONTENT_LENGTH"] = this->_request.getHeaderField("Content-Length"); //not sure
	this->_env["CONTENT_TYPE"] = this->_request.getHeaderField("Content-Type");
	
	printEnv("constructeur execCGI : before launching exec_CGI()");
	if (autoindex == false && this->_env["STATUS_CODE"].compare("200 OK") == 0)
		this->exec_method();
	Response((*this), this->_server.getClientSocket());
}

execCGI::~execCGI()
{

}

bool execCGI::tryPath(Server &server, Request &request, const std::string &target)
{
	const Location  *loc = getRelevantLocation(server.getRoutes(), target);
	const Location  *ext = getRelevantExtension(server.getRoutes(), target);
	std::string		cgi_path;

	if (loc && loc->getCGIPath().size())
		cgi_path = loc->getCGIPath();
	else if (ext && ext->getCGIPath().size())
		cgi_path = ext->getCGIPath();

    if (chdir(request.getDirPath().c_str()) == -1)
        throw chdirFailException();

	std::cout << "DIR [" << this->_request.getDirPath() << ']' << std::endl;
	this->_env["PATH_INFO"] = this->_request.getDirPath();
	if (!this->_request.getObject().empty())
		this->_env["PATH_INFO"] += '/' + this->_request.getObject();
	this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
	this->_env["SCRIPT_FILENAME"] = this->_request.getObject();
	this->_env["SCRIPT_NAME"] = this->_request.getObject();

    Autoindex autoidx(target, this->_env["PATH_INFO"]);
	if (!autoidx.path_exist())
	{
		this->_env["STATUS_CODE"] = "404 Not Found";
		return false;
	}
	if (autoidx.isDir() && !autoidx.getIndex())  // index absent
	{
		std::string tmp = autoidx.autoindex_generator();
		this->_buf_size = tmp.size();
		this->_buf = new unsigned char[tmp.size()];
		for (size_t i = 0; i < tmp.size(); i++)
			this->_buf[i] = tmp[i];
        return true;
	}

	else    // index present
	{
		if (autoidx.isDir() && autoidx.getIndex())  
		{
			this->_env["PATH_INFO"] = this->_env["PATH_INFO"] + '/' + std::string(DEFAULT_INDEX);
			this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
			this->_env["SCRIPT_FILENAME"] = DEFAULT_INDEX;
			this->_env["SCRIPT_NAME"] = DEFAULT_INDEX;
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

bool	execCGI::setPathQuery()
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
		printEnv("setPathQuery : before building path");
		this->_request.setDirPath(buildPath(_server, _request, target));
		printEnv("setPathQuery : after building path, before tryPath");
		autoindex = tryPath(_server, _request, target);
		printEnv("setPathQuery : after tryPath");
	}
	catch (std::exception &e) {
        this->_env["STATUS_CODE"] = e.what();
	}
	std::cout << "HERE" << std::endl;
	return (autoindex);
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

void	execCGI::exec_method()
{
	if (!this->_env["REQUEST_METHOD"].compare("DELETE"))
		this->exec_delete();
	if (this->_cgi)
		this->exec_CGI();
	else if (!this->_env["REQUEST_METHOD"].compare("GET"))
		this->readFile();
	else if (!this->_env["REQUEST_METHOD"].compare("POST"))
		this->_env["STATUS_CODE"] = "204 No Content";
}

void	execCGI::exec_CGI()
{
	if (this->_env["STATUS_CODE"].compare("200 OK") != 0)
		return ;
	
	this->_env["HTTP_HOST"] = this->_env["SERVER_NAME"];
	printEnv("execCGI() : debut");
	
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

void		execCGI::readFile()
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

void	execCGI::exec_delete()
{

	if (remove(this->_env["PATH_INFO"].c_str()) < 0)
		this->_env["STATUS_CODE"] = "403 Forbidden";
	else
		this->_env["STATUS_CODE"] = "204 No Content";
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

bool				execCGI::getCgi() const
{
	return this->_cgi;
}

void  execCGI::printEnv(std::string details)
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
