/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:24:53 by schene            #+#    #+#             */
/*   Updated: 2021/07/26 15:49:02 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../webserv.hpp"

class Request
{
	private:
		int									_socket;
		int									_bad_request;
		std::string							_buf;
		std::string							_line;
		std::string							_method;
		int									_method_code;
		std::string							_target;	// requested uri
		std::string							_object;	// object part of uri (filename or dirname)
		std::string							_dir_path;  // absolute path of the directory in which the object is stored
		std::string 						_http_version;
		std::map<std::string, std::string>	_headers;
		unsigned char 						*_body;
		int			 						_body_size;
		int									_max_body_size;

		void			resetHeaders();
		int				recvHeader();
		int				recvBody(const std::list<Location*> &routes);
		int				recvBodyCL(int size);
		int				recv_data();
		int				recvChunk();
		unsigned char	recv_one();
		int				gnlRequest();
		void			parseRequestLine(std::string line);
		void			parseHeaderFields(std::string line);
		int				append_body(unsigned char *buffer, int size);
		void			setMaxBodySize(const std::list<Location*> &routes);

	public:
		Request();
		~Request();

		int		parseRequest(int socket, const std::list<Location*> &routes);
		void	reset_body();

		std::string const	&getMethod() const;
		std::string const	&getTarget() const;
		std::string const	&getHTTPVersion() const;
		std::string const	&getHeaderField(std::string field_name) const;
		unsigned char 		*getBody() const;
		int			 		getBodySize() const;
		int					getBadRequest() const;
		int					getMethodCode() const;
		std::string	const	&getDirPath() const;
		void				setDirPath(std::string path);
		std::string	const	&getObject() const;
		void				setObject(std::string object);
};

#endif
