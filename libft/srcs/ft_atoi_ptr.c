/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 15:19:33 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/23 21:26:36 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_atoi_ptr(char *str, int i, int *color)
{
	int	sign;
	int	out;

	if (!str)
		return (0);
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
	*color = out * sign;
	return (i);
}
