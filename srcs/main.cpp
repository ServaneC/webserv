/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:54:51 by schene            #+#    #+#             */
/*   Updated: 2021/08/03 17:08:34 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

static void			ctr_c(int num)
{
	(void)num;
	g_ctrl_c = 1;
}

int main (int ac, char **av) //the conf file is the only possible argument 
{
	signal(SIGINT, &ctr_c);
	if (ac == 2) // use the given conf file
		Config(std::string(av[1]));
	else if (ac == 1) // use a defautl conf file
		Config("confs/vm_websev.conf");
	else // ERROR
	{
		std::cout << "ERROR" << std::endl;
		return -1;
	}
	return 0;
}