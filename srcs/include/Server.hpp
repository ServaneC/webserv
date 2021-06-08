/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/06/07 19:39:56 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../webserv.hpp"

class Server
{
	private:
		Request								&_rqst;
		Config								&_main_conf;
		std::string							_server_conf;
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
		Server(Config &conf, std::string conf_file);
		~Server();

		int			exec_accept();
		void			exec_server();
		int			getPort() const;
		std::string	getName() const;
		int			getFd() const;
		int			getSocket() const;
		int			getClientSocket() const;
		Request		&getRequest() const;
		std::string parsingName() const;
		int			parsingPort() const;
};

#endif
