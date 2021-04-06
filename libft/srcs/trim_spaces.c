/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 19:17:36 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/23 21:21:24 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*trim_spaces(char *line)
{
	int		i;
	int		j;
	int		len;
	char	*new;

	if (!line)
		return (NULL);
	len = spaceless_strlen(line);
	if (!(new = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (line[i] && j < len)
	{
		if (line[i] != ' ')
			new[j++] = line[i++];
		else
			i++;
	}
	new[j] = 0;
	free(line);
	return (new);
}
