/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:49:40 by schene            #+#    #+#             */
/*   Updated: 2021/06/07 13:50:10 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Config.hpp"

# define BUFF_SIZE 1000

Config::Config()
{

}

Config::~Config()
{
	this->_content.clear();
}

int 	Config::parseConfFile(char const *path)
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
