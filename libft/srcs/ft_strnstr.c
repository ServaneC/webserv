/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 14:37:46 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:40:19 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(char *hay, char *need, int len)
{
	int		i;
	int		j;
	char	*h;

	h = (char *)hay;
	if (need[0] == 0)
		return (h);
	i = 0;
	while (hay[i] && i < len)
	{
		j = 0;
		while (need[j] && hay[i + j] == need[j] && (i + j) < len)
		{
			if (need[j + 1] == 0)
				return (h + i);
			j++;
		}
		i++;
	}
	return (NULL);
}
