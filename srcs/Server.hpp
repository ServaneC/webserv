/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/04/02 09:47:35 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../webserv.hpp"

class Server
{
	private:
		Request								&_rqst;
		int									_port;
		std::string 						_name;
		int									_fd;
		struct sockaddr_in					_host;
		int									_socket;
		int									_addrlen;
		//std::map<std::string, std::string>	_routes;
	
	public:
		Server(std::string conf_file);
		~Server();

		int			getPort() const;
		std::string	getName() const;
		int			getFd() const;
		int			getSocket() const;
		Request		&getRequest() const;
};

#endif
