/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 17:41:56 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:43:42 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/libft.h"

char			*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	int		i;
	int		j;
	int		k;

	if (!(join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	if (s1)
	{
		while (s1[i])
			join[k++] = s1[i++];
	}
	if (s2)
	{
		while (s2[j])
			join[k++] = s2[j++];
	}
	join[k] = '\0';
	return (join);
}
