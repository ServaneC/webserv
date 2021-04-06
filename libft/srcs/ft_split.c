/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 21:58:36 by lemarabe          #+#    #+#             */
/*   Updated: 2020/01/31 17:08:22 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	word_count(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static int	w_len(char *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i] && s[i] != c)
	{
		len++;
		i++;
	}
	return (len);
}

char		**ft_split(char *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	if (!(tab = (char **)malloc(sizeof(char *) * (word_count(s, c) + 1))))
		return (NULL);
	i = 0;
	j = -1;
	while (++j < word_count(s, c) && s[0])
	{
		while (s[i] && s[i] == c)
			i++;
		if (!(tab[j] = (char *)malloc(sizeof(char) * w_len(s, c, i) + 1)))
			return (NULL);
		k = 0;
		while (s[i] && s[i] != c)
			tab[j][k++] = s[i++];
		tab[j][k] = 0;
	}
	tab[j] = 0;
	return (tab);
}
