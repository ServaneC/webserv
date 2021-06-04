/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:49:40 by schene            #+#    #+#             */
/*   Updated: 2021/05/06 14:27:25 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Config.hpp"

# define BUFF_SIZE

Config::Config()
{

}

Config::~Config()
{
	this->_content.clear();
}

void	Config::parseConfFile(char const *path)
{
	std::fstream conf_stream(path, std::ios_base::in); //open file for reading
	//char *line = NULL;
	// int ret;
	// struct stat info;
	//if (!(line = malloc(sizeof(char) * BUFF_SIZE)))
	//	return -1;
	//while (1)
	//{
	//	conf_stream.getline(line, BUFF_SIZE);
	//	if ()
	//}
	// if (fd < 0)
	// {
	// 	perror("can't open conf file");
	// 	exit(EXIT_FAILURE);
	// }
	// fstat(fd, &info);
	// this->_content.reserve(info.st_size);
	// std::cout << "size = |" << info.st_size << '|' << std::endl;
	// while ((ret = get_next_line(fd, &line)) == 1)
	// {
	// 	this->_content.append(std::string(line) + "\n");
	// 	free(line);
	// 	line = NULL;
	// }
	// if (line)
	// {
	// 	this->_content.append(line);
	// 	free(line);
	// }	
	// std::cout << this->_content << std::endl;
	conf_stream.close();
}
