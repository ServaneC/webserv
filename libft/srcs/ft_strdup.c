/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 15:49:11 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/02 23:57:43 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/libft.h"

static char	*ft_strcpy(char *dest, char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char		*ft_strdup(char *str)
{
	char	*copy;

	if (!str)
		return (NULL);
	if (!(copy = (char *)malloc(sizeof(char) * ft_strlen(str) + 1)))
		return (copy);
	copy = ft_strcpy(copy, str);
	return (copy);
}
