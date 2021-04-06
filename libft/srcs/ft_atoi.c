/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 14:25:48 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/23 15:54:24 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_atoi(char *str)
{
	int	i;
	int	out;
	int	sign;

	i = 0;
	out = 0;
	sign = 1;
	while (ft_iswhitespace(str[i]))
		i++;
	if (str[i] == '-' && (str[i + 1] <= '9' && str[i + 1] >= '0'))
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+' && (str[i + 1] <= '9' && str[i + 1] >= '0'))
		i++;
	while (str[i] <= '9' && str[i] >= '0')
	{
		out = out * 10 + str[i] - '0';
		i++;
	}
	return (out * sign);
}
