/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:47:15 by schene            #+#    #+#             */
/*   Updated: 2021/07/28 17:53:17 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../../webserv.hpp"

class Config
{
	private:
		Config();
		std::string			_content;
		std::list<Server*>	_servers;
		fd_set 				_current_sockets;
		// bool				_ready;

		std::string 		singleServerConfig(size_t index);
		void				startServers();
		void				terminate_serv();

	public:
		Config(std::string conf_file);
		~Config();

		int 	readConfFile(char const *path);
		void	createServers(void);
};

#endif
