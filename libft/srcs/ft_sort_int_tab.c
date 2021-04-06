/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 12:46:47 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:46:48 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_sort_int_tab(int *tab, int size)
{
	int i;

	i = 0;
	if (!tab)
		return ;
	while (i < size - 1)
	{
		while (tab[i] > tab[i + 1])
		{
			ft_swap(&tab[i], &tab[i + 1]);
			ft_sort_int_tab(tab, size);
		}
		i++;
	}
}
