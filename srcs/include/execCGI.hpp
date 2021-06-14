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
		char								*_buf;
		char								*_bufb;
		time_t								_last_modified;
		std::map<std::string, std::string>	_env;
		char 								**_argv;

		void	setPathQuery();
		char	**env_to_char_array();
		void	exec_CGI();
		int		set_argv();
		bool	check_method();
		int		append_body(char *buffer);

	public:
		execCGI(Server &serv);
		~execCGI();

		void				free_buf();
		std::string const	&getEnvVar(std::string var_name) const;
		char				*getBuf() const;
		time_t				getLastModified() const;
};

#endif
