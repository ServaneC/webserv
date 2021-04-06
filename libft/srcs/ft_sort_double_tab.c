/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_double_tab.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 12:47:08 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:52:00 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_sort_double_tab(double *tab, int size)
{
	int i;

	i = 0;
	if (!tab)
		return ;
	while (i < size - 1)
	{
		while (tab[i] > tab[i + 1])
		{
			ft_swap_double(&tab[i], &tab[i + 1]);
			ft_sort_double_tab(tab, size);
		}
		i++;
	}
}
