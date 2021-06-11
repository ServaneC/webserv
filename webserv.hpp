/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/06/11 18:56:51 by lemarabe         ###   ########.fr       */
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
#include <poll.h>

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
// # include <cstdlib>
# include <fstream>
# include <sstream>

class	Server;
class	Request;
class	execCGI;
class	Response;
class   Config;

# include "srcs/include/Server.hpp"
# include "srcs/include/Request.hpp"
# include "srcs/include/execCGI.hpp"
# include "srcs/include/Response.hpp"
# include "srcs/include/Config.hpp"
# include "srcs/include/Location.hpp"
# include "srcs/include/parsing.hpp"
# include "srcs/include/exceptions.hpp"

#endif
