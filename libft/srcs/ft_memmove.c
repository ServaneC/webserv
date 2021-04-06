/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 11:42:54 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 17:09:52 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

static void	backwards_copy(unsigned char *d, unsigned char *s, int len)
{
	s = s + len - 1;
	d = d + len - 1;
	while (len > 0)
	{
		*d-- = *s--;
		len--;
	}
}

static void	forward_copy(unsigned char *d, unsigned char *s, int len)
{
	while (len > 0)
	{
		*d++ = *s++;
		len--;
	}
}

void		*ft_memmove(void *dst, void *src, int len)
{
	unsigned char	*d;
	unsigned char	*s;

	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	if (!src && !dst)
		return (NULL);
	if (src < dst)
		backwards_copy(d, s, len);
	else
		forward_copy(d, s, len);
	return (dst);
}
