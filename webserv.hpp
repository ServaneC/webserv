/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/06/24 15:27:24 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

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

class	Server;
class	Request;
class	execCGI;
class	Response;
class   Config;
class   Location;

# include "srcs/include/Server.hpp"
# include "srcs/include/Request.hpp"
# include "srcs/include/execCGI.hpp"
# include "srcs/include/Response.hpp"
# include "srcs/include/Config.hpp"
# include "srcs/include/Location.hpp"
# include "srcs/include/parsing.hpp"
# include "srcs/include/exceptions.hpp"
# include "srcs/include/Utils.hpp"
# include "srcs/include/Autoindex.hpp"

#endif
