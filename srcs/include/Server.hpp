/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/07/05 22:52:59 by lemarabe         ###   ########.fr       */
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
		unsigned int						_ip;
		int									_port;
		std::string 						_name;
		int									_socket;
		struct sockaddr_in					_host;
		int									_client_socket;
		int									_addrlen;
		std::string							_root;
		// std::map<std::string, std::string>	_routes;
		std::list<Location>					_routes;
		std::list<std::string>				_indexes;
		bool								_autoindex;
	
		void		start_server();
			
	public:

		Server(Config &conf, std::string conf_file);
		~Server();

		int			exec_accept();
		void		exec_server();
		int			getPort() const;
		std::string	getName() const;
		std::string	getRoot() const;
		int			getFd() const;
		int			getSocket() const;
		int			getClientSocket() const;
		Request		&getRequest() const;
		// std::list<Location> getRelevantLocations(std::string path_info);
		const Location *getRelevantExtension(std::string target);
		const Location *getRelevantLocation(std::string target);
		const std::list<Location>		&getRoutes() const;
		const std::list<std::string>	&getIndexes() const;
		bool					getAutoIndex() const;


};

#endif
