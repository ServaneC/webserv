/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:50:28 by schene            #+#    #+#             */
/*   Updated: 2021/04/09 13:53:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"
// # include <iostream>
// # include <map>
// # include "Server.hpp"
// # include "myCGI.hpp"

class Response
{
	private:
		myCGI								&_cgi;
		int									_socket;
		std::string							_buf;
		std::string							_response;
		std::map<std::string, std::string>	_headers;

		void		parse_cgi_buf();
		void		setDate();
		void		setLastModified();
		std::string	formatDate(time_t timestamp);

	public:
		Response(myCGI &my_CGI, int socket);
		~Response();
		std::string		search_header(std::string field_name) const;
};

#endif
