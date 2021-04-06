/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 13:39:50 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 13:39:15 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_fill_zero(void *s, int n)
{
	int		i;
	char	*c;

	i = 0;
	if (n == 0)
		return ;
	c = s;
	while (i < n)
	{
		c[i] = 0;
		i++;
	}
	return ;
}

void	*ft_calloc(int count, int size)
{
	void	*ptr;

	if (size == 0 || count == 0)
	{
		count = 1;
		size = 1;
	}
	if (!(ptr = malloc(size * count)))
		return (NULL);
	ft_fill_zero(ptr, size * count);
	return (ptr);
}
