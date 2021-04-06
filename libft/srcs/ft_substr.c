/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:27:49 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:38:57 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_substr(char *str, int start, int len)
{
	char	*sub;
	int		i;

	if (!str || start < 0 || start >= ft_strlen(str))
		return (NULL);
	if (!(sub = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	i = 0;
	while (len && str[start])
	{
		sub[i++] = str[start++];
		len--;
	}
	sub[i] = 0;
	return (sub);
}
