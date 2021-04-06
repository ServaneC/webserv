/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 11:24:42 by schene            #+#    #+#             */
/*   Updated: 2021/03/25 09:55:11 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_lstadd_back(t_list **alst, t_list *_new)
{
	t_list	*last;

	if (alst == NULL)
		return ;
	last = *alst;
	if (*alst == NULL)
		*alst = _new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = _new;
	}
}
