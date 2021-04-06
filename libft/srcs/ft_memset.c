/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 11:35:14 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 17:09:01 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *b, int c, int len)
{
	unsigned char	*str;

	str = b;
	while (len-- > 0)
		*str++ = c;
	return (b);
}
