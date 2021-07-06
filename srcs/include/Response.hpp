/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:50:28 by schene            #+#    #+#             */
/*   Updated: 2021/06/16 00:54:18 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../webserv.hpp"

class Response
{
	private:
		execCGI								&_cgi;
		int									_socket;
		unsigned char 						*_buf;
		int									_idx;
		int									_buf_size;
		std::string							_response;
		char								*_response_c;
		std::map<std::string, std::string>	_headers;
		std::map<int,int>					_socketsMap;

		void		parse_cgi_buf();
		void		setDate();
		void		setLastModified();
		std::string	formatDate(time_t timestamp);
		void		writeStatusLine();
		void		send_response();
		void		format_header();
		void		check_content_type();

	public:
		Response(execCGI &my_CGI, int socket);
		~Response();
};

#endif
