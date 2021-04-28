/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 21:28:17 by lemarabe          #+#    #+#             */
/*   Updated: 2021/04/23 15:33:28 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/libft.h"

int		read_next_line(int fd, char **stock)
{
	char	*tmp;
	char	buf[BUFFER_SIZE + 1];
	int		ret;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= 2147483647)
		return (-1);
	if ((ret = read(fd, buf, BUFFER_SIZE)) && ret > 0)
	{
		tmp = ft_strdup(*stock);
		free(*stock);
		buf[ret] = 0;
		*stock = ft_strjoin(tmp, buf);
		free(tmp);
	}
	return (ret);
}

void	update_strings(char **line, char **stock, int limit)
{
	char	*tmp;
	char	*to_add;

	tmp = *line;
	to_add = ft_substr(*stock, 0, limit);
	*line = ft_strjoin(tmp, to_add);
	free(tmp);
	free(to_add);
	tmp = ft_strdup(*stock);
	free(*stock);
	*stock = ft_substr(tmp, (limit + 1), (ft_strlen(tmp) - limit - 1));
	free(tmp);
}

int		get_next_line(int fd, char **line)
{
	static t_stock	save;
	int				limit;
	int				ret;

	if (fd < 0 || line == NULL)
		return (-1);
	if ((ret = read_next_line(fd, &(save.stock[fd]))) == -1)
		return (-1);
	if (save.i[fd] == 0)
		*line = ft_strdup("");
	limit = seek_nl(save.stock[fd]);
	save.i[fd] = 1;
	if (save.stock[fd] && save.stock[fd][limit] == '\n')
		save.i[fd] = 0;
	update_strings(line, &(save.stock[fd]), limit);
	if (save.i[fd] == 0)
		return (1);
	if (ret < BUFFER_SIZE && !save.stock[fd])
		return (0);
	return (get_next_line(fd, line));
}
