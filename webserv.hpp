/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:34:32 by schene            #+#    #+#             */
/*   Updated: 2021/04/07 11:26:15 by schene           ###   ########.fr       */
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

# include <sys/wait.h>
# include <iostream>
# include <sys/types.h> 
# include <sys/socket.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <map>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>

class	Server;
class	Request;
class	myCGI;
class	Response;


# include "srcs/Server.hpp"
# include "srcs/Request.hpp"
# include "srcs/myCGI.hpp"
# include "srcs/Response.hpp"

#endif
