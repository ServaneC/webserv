/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:49:40 by schene            #+#    #+#             */
/*   Updated: 2021/06/07 19:45:31 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Config.hpp"

# define BUFF_SIZE 1000

Config::Config() {}

Config::Config(std::string conf_file) : _servers()
{
	readConfFile(conf_file.c_str());
	createServers();
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
	
	if (!(line = (char *)malloc(sizeof(char) * BUFF_SIZE)))
		return -1;
	while (1)
	{
		conf_stream.getline(line, BUFF_SIZE);
		this->_content.append(line);
		this->_content.push_back('\n');
		memset(line, '\0', BUFF_SIZE);
		if (conf_stream.eof())
			break ;
	}
	std::cout << "===== CONF =====" << std::endl;
	std::cout << this->_content << std::endl;
	std::cout << "================" << std::endl;
	conf_stream.close();
	return 1;
}

std::string Config::singleServerConfig(size_t index)
{
	size_t open_bracket = _content.find_first_not_of("serv \t\n\r\v\f", index);
	// std::cout << open_bracket << " = " << _content[open_bracket] << std::endl;
	size_t close_bracket = findClosingBracket(_content, open_bracket);
	// std::cout << close_bracket << " = " << _content[close_bracket] << std::endl;
	
	if (_content[open_bracket] == '{' && _content[close_bracket] == '}')
		return (_content.substr(open_bracket, close_bracket - open_bracket + 1));
	return (std::string());
}

void	Config::createServers(void)
{
	size_t last_found = _content.find("server", 0);
	
	while (last_found < _content.size() && last_found < std::string::npos)
	{
		std::string single_server_conf = singleServerConfig(last_found);
		// std::cout << "HERE'S WHAT I JUST CREATED :: " << single_server_conf << "\nOK BYE" << std::endl;
		
		if (!single_server_conf.empty())
		{
			// std::cout << "adding one server" << std::endl;
			_servers.push_back(new Server(*this, single_server_conf));
			// std::cout << "added one server" << std::endl;
		}
		last_found = _content.find("server", last_found + 1);
	}
}