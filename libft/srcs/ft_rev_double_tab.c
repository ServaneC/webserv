/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_double_tab.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 12:50:33 by lemarabe          #+#    #+#             */
/*   Updated: 2020/02/20 12:51:16 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_rev_double_tab(double *tab, int size)
{
	int i;

	if (!tab)
		return ;
	i = 0;
	while (i < (size / 2))
	{
		ft_swap_double(&tab[i], &tab[size - i - 1]);
		i++;
	}
}
