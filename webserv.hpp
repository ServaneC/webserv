/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/06/09 02:47:06 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// # include <arpa/inet.h>
// # include <stdlib.h>
# include <sys/wait.h>
# include <stdio.h>
# include <sys/types.h> 
# include <sys/socket.h>
# include <sys/stat.h>
# include <unistd.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <errno.h>

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>
# include <cctype>
// # include <cstdlib>
# include <fstream>
# include <sstream>

class	Server;
class	Request;
class	execCGI;
class	Response;
class   Config;

size_t          findClosingBracket(const std::string str, size_t begin);
std::string     getScope(const std::string str, size_t index);
// std::string     getScope(const std::string str, size_t index, const char *charset);
unsigned char   getValueBetweenPoints(const std::string str, size_t *index);
bool            isCommentLine(const char *line);

# include "srcs/include/Server.hpp"
# include "srcs/include/Request.hpp"
# include "srcs/include/execCGI.hpp"
# include "srcs/include/Response.hpp"
# include "srcs/include/Config.hpp"
// # include "srcs/include/IPAddress.hpp"
# include "srcs/include/exceptions.hpp"

#endif
