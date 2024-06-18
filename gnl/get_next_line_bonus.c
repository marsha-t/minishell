/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 07:07:10 by mateo             #+#    #+#             */
/*   Updated: 2024/01/17 13:42:37 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_lststrcat(int len, t_list **head, char *fullline)
{
	int		i;
	int		j;

	i = 0;
	while (i < len && ((*head)->next))
	{
		j = 0;
		while ((*head)->str[j])
			fullline[i++] = (*head)->str[j++];
		*head = (*head)->next;
	}
	if (i == len)
		ft_lstclear(&(*head)->prev, 0);
	else
	{
		j = 0;
		while ((*head)->str[j])
			fullline[i++] = (*head)->str[j++];
		ft_lstclear(head, 0);
	}
	return (fullline);
}

char	*ft_lststrcat_setup(t_list **head, t_list *last)
{
	char	*fullline;
	int		len;

	len = ft_lststrlen(*head, last);
	if (!len)
		return (ft_lstclear(head, 1));
	fullline = malloc((len + 1) * sizeof(char));
	if (!fullline)
		return (ft_lstclear(head, 1));
	fullline[len] = '\0';
	fullline = ft_lststrcat(len, head, fullline);
	return (fullline);
}

char	*gnl_setup(int fd, char **newline, int *read_r, t_list **head)
{
	if (fd < 0 || BUFFER_SIZE < 1 || fd >= OPEN_MAX)
		return (0);
	*newline = 0;
	*read_r = 1;
	if (*read_r >= 0 && *head)
		*newline = ft_strchr((*head)->str, '\n');
	if (read(fd, 0, 0) < 0)
		return (ft_lstclear(head, 1));
	return ("1");
}

char	*get_next_line(int fd)
{
	static t_list	*head_array[OPEN_MAX];
	t_list			*last_array[OPEN_MAX];
	int				read_r;
	char			*newline;

	if (!gnl_setup(fd, &newline, &read_r, &head_array[fd]))
		return (0);
	last_array[fd] = head_array[fd];
	while (read_r > 0 && !newline)
	{
		last_array[fd] = ft_lstaddnew(&head_array[fd], BUFFER_SIZE + 1);
		if (!last_array[fd])
			return (ft_lstclear(&head_array[fd], 1));
		read_r = read(fd, last_array[fd]->str, BUFFER_SIZE);
		last_array[fd]->str[read_r] = '\0';
		newline = ft_strchr(last_array[fd]->str, '\n');
	}
	if (read_r == 0)
		last_array[fd]->eof = 1;
	if (read_r >= 0 && newline)
		last_array[fd] = ft_lstsplit(last_array[fd], newline);
	if (read_r >= 0 && last_array[fd])
		return (ft_lststrcat_setup(&head_array[fd], last_array[fd]));
	return (ft_lstclear(&head_array[fd], 1));
}
