/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceless_strlen.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 19:23:42 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 19:55:23 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		spaceless_strlen(char *line)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] != ' ')
			count++;
		i++;
	}
	return (count);
}
