/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:19:12 by mateo             #+#    #+#             */
/*   Updated: 2024/01/17 14:28:30 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H

# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_list
{
	struct s_list	*next;
	struct s_list	*prev;
	char			*str;
	int				nl;
	int				eof;
}	t_list;

char	*ft_strchr(const char *s, int c);
void	*ft_lstclear(t_list **head, int clear);
t_list	*ft_lstaddnew(t_list **head, int blank);
t_list	*ft_lstsplit(t_list *last, char *newline);
int		ft_lststrlen(t_list *head, t_list *last);

char	*ft_lststrcat(int len, t_list **head, char *fullline);
char	*ft_lststrcat_setup(t_list **head, t_list *last);
char	*gnl_setup(int fd, char **newline, int *read_r, t_list **head);
char	*get_next_line(int fd);

#endif