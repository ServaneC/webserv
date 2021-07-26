/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/07/26 14:06:10 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../webserv.hpp"

class Server
{
	private:

		Request								&_rqst;
		Config 								&_main_conf;
		std::string							_server_conf;
		unsigned int						_ip;
		int									_port;
		std::string 						_name;
		int									_socket;
		struct sockaddr_in					_host;
		int									_client_socket;
		int									_addrlen;
		const Location						*_root;
		std::list<Location*>					_routes;
	
		void		start_server();
		Server();
			
	public:

		Server(Config &conf, std::string conf_file);
		~Server();

		int			exec_accept();
		int			exec_server();
		int			getPort() const;
		std::string	getName() const;
		std::string	getRoot() const;
		int			getFd() const;
		int			getSocket() const;
		int			getClientSocket() const;
		Request		&getRequest() const;
		const Location *getRelevantLocation(std::string target);
		const std::list<Location*>		&getRoutes() const;
		const Location	&getRootLocation() const;


};

#endif
