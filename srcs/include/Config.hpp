/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 09:47:15 by schene            #+#    #+#             */
/*   Updated: 2021/04/30 10:14:00 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../../webserv.hpp"

class Config
{
	private:
		std::string 	_content;

	public:
		Config();
		~Config();

		void	parseConfFile(char const *path);
};

#endif
