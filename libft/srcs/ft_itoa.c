/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 22:03:58 by lemarabe          #+#    #+#             */
/*   Updated: 2019/10/21 12:59:30 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int		nbr_len(int nbr)
{
	int		i;
	long	p;

	i = 0;
	if (nbr <= 0)
	{
		p = -(long)nbr;
		i = 1;
	}
	else
		p = (long)nbr;
	while (p)
	{
		p = p / 10;
		i++;
	}
	return (i);
}

static char		*ft_strrev(char *str)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	while (str[i])
		i++;
	i--;
	while (i > j)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
		i--;
		j++;
	}
	return (str);
}

static char		*print_nbr(char *str, long p, int n_len)
{
	int		i;

	i = 0;
	while (p)
	{
		str[i] = p % 10 + '0';
		p = p / 10;
		i++;
	}
	str[n_len] = 0;
	return (str);
}

char			*ft_itoa(int nbr)
{
	char	*str;
	long	p;
	int		n_len;

	n_len = nbr_len(nbr);
	if (!(str = (char *)malloc(sizeof(char) * n_len + 1)))
		return (NULL);
	if (nbr < 0)
	{
		str[n_len - 1] = '-';
		p = -(long)nbr;
	}
	else if (nbr == 0)
	{
		str[0] = '0';
		str[1] = 0;
		return (str);
	}
	else
		p = (long)nbr;
	str = print_nbr(str, p, n_len);
	return (ft_strrev(str));
}
