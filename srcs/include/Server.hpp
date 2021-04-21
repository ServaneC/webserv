/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/04/20 14:20:31 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../webserv.hpp"

class Server
{
	private:
		Request								&_rqst;
		// std::string							_buf;
		int									_port;
		std::string 						_name;
		int									_socket;
		struct sockaddr_in					_host;
		int									_client_socket;
		int									_addrlen;
		//std::map<std::string, std::string>	_routes;
		void	start_server();
			
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
