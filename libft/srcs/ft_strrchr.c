/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 23:34:30 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 16:47:45 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(char *s, int c)
{
	int		i;
	int		last_oc;
	char	a;
	char	*str;

	last_oc = 0;
	i = 0;
	a = (char)c;
	str = (char *)s;
	while (s[i])
	{
		if (s[i] == a)
			last_oc = i;
		i++;
	}
	if (c == 0)
		return (str + i);
	else if (last_oc || (last_oc == 0 && s[0] == a))
		return (str + last_oc);
	else
		return (NULL);
}
