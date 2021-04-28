/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:54:51 by schene            #+#    #+#             */
/*   Updated: 2021/04/21 10:21:39 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int main (int ac, char **av) //the conf file is the only possible argument 
{
	if (ac == 2) // use the given conf file
		Server(std::string(av[1]));
	else if (ac == 1) // use a defautl conf file
		Server("default.conf");
	else	// ERROR
	{
		// std::cout << "ERROR" << std::endl;
		return -1;
	}
	return 0;
}