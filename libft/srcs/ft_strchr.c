/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 14:28:40 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 16:47:45 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(char *s, int c)
{
	int		i;
	char	a;
	char	*str;

	i = 0;
	a = (char)c;
	str = (char *)s;
	while (s[i])
	{
		if (s[i] == a)
			return (str + i);
		i++;
	}
	if (c == 0)
		return (str + i);
	else
		return (NULL);
}
