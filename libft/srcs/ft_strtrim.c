/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 17:58:06 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 17:08:22 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

static int		check(char *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int		alloc_size(char *s1, char *set)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (s1[i] && check(set, s1[i]))
		i++;
	while (s1[i])
	{
		i++;
		size++;
	}
	if (size == 0)
		return (size);
	i--;
	while (check(set, s1[i]) && i >= 0)
	{
		size--;
		i--;
	}
	return (size);
}

char			*ft_strtrim(char *s1, char *set)
{
	char	*trim;
	int		i;
	int		j;
	int		size;

	if (!s1)
		return (NULL);
	size = alloc_size(s1, set);
	if (!(trim = malloc(sizeof(char) * size + 1)))
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] && check(set, s1[i]))
		i++;
	while (j < size)
		trim[j++] = s1[i++];
	trim[j] = '\0';
	return (trim);
}
