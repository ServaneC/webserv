/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:20:51 by schene            #+#    #+#             */
/*   Updated: 2021/06/09 03:28:28 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../webserv.hpp"
# include "IPAddress.hpp"

class Server
{
	private:

		Request								&_rqst;
		Config								&_main_conf;
		std::string							_server_conf;
		// std::string							_buf;
		IPA_t								_ipAddress;
		int									_port;
		std::string 						_name;
		int									_socket;
		struct sockaddr_in					_host;
		int									_client_socket;
		int									_addrlen;
		std::map<std::string, std::string>	_routes;
	
		void		start_server();
		void 		storeIPAddress(size_t index);
		void		parsingIPAddress();
		std::string parsingName() const;
		void		parsingLocations();
			
	public:

		Server(Config &conf, std::string conf_file);
		~Server();
		int			getPort() const;
		std::string	getName() const;
		int			getFd() const;
		int			getSocket() const;
		Request		&getRequest() const;
};

#endif
