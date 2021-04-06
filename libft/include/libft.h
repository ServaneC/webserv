/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 11:33:29 by lemarabe          #+#    #+#             */
/*   Updated: 2021/03/25 09:54:51 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include "get_next_line.h"

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

void				*ft_memset(void *b, int c, int len);
void				ft_bzero(void *s, int n);
void				*ft_memcpy(void *dst, void *src, int n);
void				*ft_memccpy(void *dst, void *src, int c, int n);
void				*ft_memmove(void *dst, void *src, int len);
void				*ft_memchr(void *s, int c, int n);
int					ft_memcmp(void *s1, void *s2, int n);
void				*ft_calloc(int count, int size);
int					ft_strlen(char *str);
int					ft_toupper(int c);
int					ft_tolower(int c);
char				*ft_strchr(char *s, int c);
char				*ft_strrchr(char *s, int c);
int					ft_strncmp(char *s1, char *s2, int n);
int					ft_strlcpy(char *dest, char *src, int dstsize);
int					ft_strlcat(char *dst, char *src, int size);
char				*ft_strnstr(char *hay, char *need, int n);
char				*ft_strdup(char *str);
char				*ft_substr(char *s, int start, int len);
char				*new_str_from(char *str, int i);
char				*ft_strjoin(char *s1, char *s2);
char				*ft_strtrim(char *s1, char *set);
char				**ft_split(char *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char *s, char (*f)(unsigned int, char));
int					seek_nl(char *str);
char				*trim_spaces(char *line);
int					spaceless_strlen(char *line);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putstr(char *s);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
int					get_next_line(int fd, char **line);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_isspace(int i);
int					ft_iswhitespace(int i);
void				ft_sort_int_tab(int *tab, int size);
void				ft_swap(int *a, int *b);
void				ft_sort_double_tab(double *tab, int size);
void				ft_swap_double(double *a, double *b);
void				ft_rev_int_tab(int *tab, int size);
void				ft_rev_double_tab(double *tab, int size);

int					ft_abs(int nb);
int					ft_atoi(char *str);
int					ft_atoi_from(char *str, int i);
int					ft_atoi_ptr(char *str, int i, int *color);

void				ft_lstadd_back(t_list **alst, t_list *_new);
void				ft_lstadd_front(t_list **alst, t_list *_new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
t_list				*ft_lstnew(void *content);
void				ft_lstclear(t_list **lst, void (*del)(void *));
int					ft_lstsize(t_list *lst);
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstmap(t_list *lst,
					void *(*f)(void *), void (*del)(void *));

#endif
