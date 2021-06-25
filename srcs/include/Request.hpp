/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:53 by schene            #+#    #+#             */
/*   Updated: 2021/06/25 14:42:23 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../webserv.hpp"

class Request
{
	private:
		int									_socket;
		bool								_bad_request;
		std::string							_buf;
		std::string							_line;
		std::string							_method;
		int									_method_code;
		std::string							_target;
		std::string 						_http_version;
		std::map<std::string, std::string>	_headers;
		unsigned char 						*_body;
		int			 						_body_size;

		void			resetHeaders();
		int				recvHeader();
		int				recvBody(int size);
		int				recvChunk();
		unsigned char	recv_one();
		int				gnlRequest();
		void			parseRequestLine(std::string line);
		void			parseHeaderFields(std::string line);
		int				append_body(unsigned char *buffer, int size);

	public:
		Request();
		~Request();

		int		parseRequest(int socket);
		void	reset_body();

		std::string const	&getMethod() const;
		std::string const	&getTarget() const;
		std::string const	&getHTTPVersion() const;
		std::string const	&getHeaderField(std::string field_name) const;
		unsigned char 		*getBody() const;
		int			 		getBodySize() const;
		bool				getBadRequest() const;
		int					getMethodCode() const;
};

#endif
