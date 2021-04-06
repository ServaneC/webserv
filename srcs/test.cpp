/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:14:33 by schene            #+#    #+#             */
/*   Updated: 2021/04/02 14:01:26 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../libft/include/libft.h"
#include <iostream>
#include <sys/time.h>

#include <stdio.h>
#include <time.h>

int main(void) 
// {
// //   char buf[1000];
// //   time_t now = time(0);
// //   struct tm tm = *gmtime(&now);

//   strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
//   std::cout << buf << std::endl;
//   return 0;
// }


{
	char			buffer[30];
	struct timeval	tv;
	struct tm		tm;

	gettimeofday(&tv, NULL);
	tm = *gmtime(&tv.tv_sec);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	std::cout << buffer << std::endl;
}

// int main ()
// {
// 	struct timeval *time_s = new struct timeval;
// 	int time = gettimeofday(time_s, NULL);
// 	char buffer[64];

// 	std::cout << "ret = " << time << std::endl;
// 	std::cout << "tv_sec = " << time_s->tv_sec << std::endl;
// 	std::cout << "tv_usec = " << time_s->tv_usec << std::endl;
	
// 	// strftime (buffer, sizeof buffer, "", info);
// 	delete time_s;
// 	return 1;
// }