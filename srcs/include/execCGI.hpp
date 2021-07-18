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
	
	
public:
		execCGI(Server &serv);
		~execCGI();

		// void				exec_CGI(execMethod &exec_method);
		void				free_buf();
		std::string const	&getEnvVar(std::string var_name) const;
		std::string const	&getRequestHeader(std::string field_name) const;
		unsigned char		*getBuf() const;
		int					getBufSize() const;
		time_t				getLastModified() const;
		bool				getCgi() const;

};

#endif
