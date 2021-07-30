/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/18 17:09:30 by schene            #+#    #+#             */
/*   Updated: 2021/07/18 17:22:18 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef execRequest_HPP
# define execRequest_HPP

# include "../../webserv.hpp"
# define READ_SIZE 2000

class execRequest
{
	private:
		Server								&_server;
		Request								&_request;
		int									_error_code;
		int									_request_buf_start;
		int									_request_buf_size;
		unsigned char						*_buf;
		int									_buf_size;
		time_t								_last_modified;
		std::map<std::string, std::string>	_env;
		char 								**_argv;
		std::list<Location>					_location_list;
		bool								_cgi;
		std::string							_file_ext;
		std::string							_upload_path;
		std::map<int, std::string>			_error_pages;

		// execRequest
		bool	tryPath(Server &server, Request &request, const std::string &target);
		void	setPathName(std::string path_info, std::string object);
		bool	setPathQuery();
		int		set_argv();
		void	append_body(unsigned char *buffer, int size);
		
		// execMethod
		void	exec_method();
		void	readFile();
		void	exec_delete();
		void	serveErrorPage();
		void	setStatusCode();

		// execPostMethod
		void			exec_post();
		std::string 	parseRequestBody();
		void			upload_file();
	
		// execCGI
		char	**env_to_char_array();
		void	exec_CGI();

		void  	printEnv(std::string);
	
public:
		execRequest(Server &serv);
		~execRequest();

		void				free_buf();
		std::string const	&getEnvVar(std::string var_name) const;
		std::string const	&getRequestHeader(std::string field_name) const;
		unsigned char		*getBuf() const;
		int					getBufSize() const;
		time_t				getLastModified() const;
		bool				getCgi() const;

};

#endif
