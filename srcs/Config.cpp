/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:49:40 by schene            #+#    #+#             */
/*   Updated: 2021/07/26 14:36:20 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Config.hpp"

# define BUFF_SIZE 1000
static int     g_ctrl_c;


Config::Config() { g_ctrl_c = 0;}

Config::Config(std::string conf_file) : _servers()
{
	readConfFile(conf_file.c_str());
	createServers();
	startServers();
}

Config::~Config()
{
	this->_content.clear();
	this->_servers.clear();
}

int 	Config::readConfFile(char const *path)
{
	std::fstream conf_stream(path, std::ios_base::in); //open file for reading
	char *line = NULL;
	
	try {
		line = new char [BUFF_SIZE];
	}
	catch (std::exception &e) {
        std::cout << e.what() << std::endl;
		return (-1);
	}
	while (1)
	{
		conf_stream.getline(line, BUFF_SIZE);
		if (!isCommentLine(line))
		{
			this->_content.append(line);
			this->_content.push_back('\n');
		}
		memset(line, '\0', BUFF_SIZE);
		if (conf_stream.eof())
			break ;
	}
	conf_stream.close();
	return 1;
}

std::string Config::singleServerConfig(size_t index)
{
	size_t open_bracket = this->_content.find_first_not_of("serv \t\n\r\v\f", index); // why "\t\n\r\v\f" ?
	size_t close_bracket = findClosingBracket(_content, open_bracket);
	
	if (this->_content[open_bracket] == '{' && _content[close_bracket] == '}')
		return (this->_content.substr(open_bracket, close_bracket - open_bracket + 1));
	return (std::string());
}

void	Config::createServers(void)
{
	size_t last_found = _content.find("server", 0);
	
	while (last_found < _content.size() && last_found < std::string::npos)
	{
		last_found += 7;
		std::string single_server_conf = getScope(_content, last_found);
		
		if (!single_server_conf.empty())
		{
			Server *to_push = new Server(*this, single_server_conf);
			this->_servers.push_back(to_push);
		}
		last_found = _content.find("server", last_found);
	}
}

void				Config::terminate_serv()
{
	std::list<Server*>::iterator it;

	for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		if ((*it)->getClientSocket() > -1)
			close ((*it)->getClientSocket());
		if ((*it)->getSocket() > -1)
			close ((*it)->getSocket());
	}
	exit(EXIT_SUCCESS);
}

void	Config::startServers()
{
	int ret;
    fd_set read_sockets, write_sockets;
	std::list<Server*>::iterator it;

	FD_ZERO(&this->_current_sockets);
	for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		if ((*it)->getSocket() > 0)
			FD_SET((*it)->getSocket(), &this->_current_sockets);
	}

    while (1)
    {
		int select_ret;
		FD_ZERO(&read_sockets);
		FD_ZERO(&write_sockets);
		std::memcpy(&read_sockets, &this->_current_sockets, sizeof(this->_current_sockets));
		std::memcpy(&write_sockets, &this->_current_sockets, sizeof(this->_current_sockets));

		try
		{
			select_ret = select(FD_SETSIZE, &read_sockets, &write_sockets, NULL, NULL);
			if (select_ret < 0)
			{
				for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
					close ((*it)->getSocket());
				throw InternalServerError();
			}
			else if (select_ret > 0)
			{
				for (int i = 0; i < FD_SETSIZE; i++)
				{
					if (FD_ISSET(i, &read_sockets))
					{
						for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
						{
							if (i == (*it)->getSocket() && (*it)->getClientSocket() < 0)
							{
								ret = (*it)->exec_accept();
								FD_SET(ret, &this->_current_sockets);
							}
							if (FD_ISSET(i, &write_sockets) && (*it)->getClientSocket() != -1)
							{
								(*it)->exec_server();
								FD_CLR(i, &this->_current_sockets);
							}
						}
					}
				}
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			exit(EXIT_FAILURE);
		}
	}
	for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
		close ((*it)->getSocket());
}
