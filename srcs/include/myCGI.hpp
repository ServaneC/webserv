/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myCGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:30:48 by schene            #+#    #+#             */
/*   Updated: 2021/04/01 15:24:13 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef myCGI_HPP
# define myCGI_HPP

# include "../../webserv.hpp"
# define READ_SIZE 2000

class myCGI
{
	private:
		// int									_fd;
		Server								&_server;
		Request								&_request;
		char								*_buf;
		std::map<std::string, std::string>	_env;

		void	setPathQuery();
		char	**env_to_char_array();
		void	execCGI();
		void	execGET();

	public:
		myCGI(Server &serv);
		~myCGI();

		std::string const	&getEnvVar(std::string var_name) const;
		std::string const 	&getBuf() const;
};

#endif
