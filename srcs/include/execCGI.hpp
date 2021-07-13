/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execCGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:30:48 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 15:24:13 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef execCGI_HPP
# define execCGI_HPP

# include "../../webserv.hpp"
# define READ_SIZE 2000

class execCGI
{
	private:
		Server								&_server;
		Request								&_request;
		unsigned char						*_buf;
		int									_buf_size;
		time_t								_last_modified;
		std::map<std::string, std::string>	_env;
		char 								**_argv;
		std::list<Location>					_location_list;
		bool								_cgi;

		bool	setPathQuery();
		char	**env_to_char_array();
		void	exec_CGI();
		void	exec_method();
		void	exec_delete();
		int		set_argv();
		bool	check_method();
		void	readFile();
		void	append_body(unsigned char *buffer, int size);
		void  	printEnv(std::string);
		bool	tryPath(Server &server, Request &request, const std::string &target);
		void	serveErrorPage(const std::string &path);
	
public:
		execCGI(Server &serv);
		~execCGI();

		void				free_buf();
		std::string const	&getEnvVar(std::string var_name) const;
		std::string const	&getRequestHeader(std::string field_name) const;
		unsigned char		*getBuf() const;
		int					getBufSize() const;
		time_t				getLastModified() const;
		bool				getCgi() const;

};

#endif
