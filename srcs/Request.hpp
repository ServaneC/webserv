/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:53 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 21:58:44 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../webserv.hpp"

// # include <iostream>
// # include <unistd.h>
// # include <map>
// extern "C" 
// {
//   #include "../libft/include/libft.h"
// }

class Request
{
	private:
		int									_fd;
		std::string							_method;
		std::string							_target;
		std::string 						_http_version;
		std::map<std::string, std::string>	_headers;

	public:
		Request();
		~Request();
		// Request 	&operator=(Request const &rhs);

		int		parseRequest(int fd);

		std::string const	&getMethod() const;
		std::string const	&getTarget() const;
		std::string const	&getHTTPVersion() const;
		std::string const	getHeaderField(std::string field_name) const;
};

#endif
