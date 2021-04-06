/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_str_from.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 12:35:23 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:36:50 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*new_str_from(char *str, int i)
{
	int		len;
	char	*new;

	if (!str)
		return (NULL);
	if (i > ft_strlen(str))
		return (NULL);
	len = 0;
	while (str[i])
	{
		i++;
		len++;
	}
	if (!(new = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	new[len] = 0;
	while (i >= 0)
		new[--len] = str[--i];
	return (new);
}
