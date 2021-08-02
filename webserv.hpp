/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/08/02 19:36:29 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

static int     g_ctrl_c;

// 42 MAC
// # define CGI_PATH "/Users/schene/.brew/bin/php-cgi"
// OTHER MAC
// # define CGI_PATH "/usr/local/Cellar/php/8.0.7/bin/php-cgi"
// VM or LINUX
// # define CGI_PATH "/usr/bin/php-cgi"

# include <sys/wait.h>
# include <stdio.h>
# include <sys/types.h> 
# include <sys/socket.h>
# include <sys/stat.h>
# include <unistd.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <vector>
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>
# include <cctype>
# include <cstdlib>
# include <fstream>
# include <csignal>
# include <sstream>
# include <limits>

class	Server;
class	Request;
class	execCGI;
class	execRequest;
class	Response;
class   Config;
class   Location;

# include "srcs/include/Server.hpp"
# include "srcs/include/Request.hpp"
# include "srcs/include/execRequest.hpp"
# include "srcs/include/Response.hpp"
# include "srcs/include/Config.hpp"
# include "srcs/include/Location.hpp"
# include "srcs/include/parsing.hpp"
# include "srcs/include/exceptions.hpp"
# include "srcs/include/Utils.hpp"
# include "srcs/include/Autoindex.hpp"

#endif
