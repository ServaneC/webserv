/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/04/23 09:29:29 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

extern "C" 
{
  #include "libft/include/libft.h"
}

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
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>

class	Server;
class	Request;
class	myCGI;
class	Response;

# include "srcs/include/Server.hpp"
# include "srcs/include/Request.hpp"
# include "srcs/include/myCGI.hpp"
# include "srcs/include/Response.hpp"

#endif
