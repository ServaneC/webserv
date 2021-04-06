/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 15:37:36 by schene            #+#    #+#             */
/*   Updated: 2019/10/19 14:26:23 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*tmp;

	if (!(lst && f && del))
		return (NULL);
	if (!(newlst = malloc(sizeof(t_list))))
		return (NULL);
	newlst->content = (*f)(lst->content);
	newlst->next = lst->next;
	tmp = newlst;
	while (lst)
	{
		newlst = newlst->next;
		lst = lst->next;
		if (!(newlst = malloc(sizeof(t_list))))
		{
			ft_lstclear(&tmp, del);
			return (NULL);
		}
		newlst->content = (*f)(lst->content);
	}
	newlst->next = NULL;
	return (tmp);
}
