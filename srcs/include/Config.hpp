/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:47:15 by schene            #+#    #+#             */
/*   Updated: 2021/06/08 20:13:08 by lemarabe         ###   ########.fr       */
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

	public:
		Config(std::string conf_file);
		~Config();

		int 	readConfFile(char const *path);
		void	createServers(void);
};

#endif
