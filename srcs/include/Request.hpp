/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:53 by schene            #+#    #+#             */
/*   Updated: 2021/06/22 14:50:57 by schene           ###   ########.fr       */
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
		// std::string							_request;
		std::string							_buf;
		std::string							_line;
		std::string							_method;
		int									_method_code;
		std::string							_target;
		std::string 						_http_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		void		resetHeaders();
		int			recvHeader();
		int			recvData(int size, int mode);
		int			gnlRequest();
		void		parseRequestLine(std::string line);
		void		parseHeaderFields(std::string line);
		char		*free_null_line(char *line);

	public:
		Request();
		~Request();

		int		parseRequest(int socket);

		std::string const	&getMethod() const;
		std::string const	&getTarget() const;
		std::string const	&getHTTPVersion() const;
		std::string const	&getHeaderField(std::string field_name) const;
		std::string const	&getBody() const;
		bool				getBadRequest() const;
		int					getMethodCode() const;
};

#endif
