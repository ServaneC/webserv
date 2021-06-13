/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:53 by schene            #+#    #+#             */
/*   Updated: 2021/06/11 17:45:03 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../webserv.hpp"

class Request
{
	private:
		//int									_fd;
		int									_socket;
		bool								_bad_request;
		std::string							_request;
		std::string							_line;
		std::string							_method;
		int									_method_code;
		std::string							_target;
		std::string 						_http_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		int			recvRequest();
		int			recvBody();
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
