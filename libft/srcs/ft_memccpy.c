/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 11:40:52 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:58:15 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memccpy(void *dst, void *src, int c, int n)
{
	unsigned char		u;
	unsigned char		*d;
	const unsigned char	*s;
	int					i;

	u = (unsigned char)c;
	d = dst;
	s = src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		if (s[i] == u)
			return (d + i + 1);
		i++;
	}
	return (NULL);
}
