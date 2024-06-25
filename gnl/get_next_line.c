/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 07:07:10 by mateo             #+#    #+#             */
/*   Updated: 2024/01/17 14:28:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	if (fd < 0 || BUFFER_SIZE < 1)
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
	static t_list	*head;
	t_list			*last;
	int				read_r;
	char			*newline;

	if (!gnl_setup(fd, &newline, &read_r, &head))
		return (0);
	last = head;
	while (read_r > 0 && !newline)
	{
		last = ft_lstaddnew(&head, BUFFER_SIZE + 1);
		if (!last)
			return (ft_lstclear(&head, 1));
		read_r = read(fd, last->str, BUFFER_SIZE);
		last->str[read_r] = '\0';
		newline = ft_strchr(last->str, '\n');
	}
	if (read_r == 0)
		last->eof = 1;
	if (read_r >= 0 && newline)
		last = ft_lstsplit(last, newline);
	if (read_r >= 0 && last)
		return (ft_lststrcat_setup(&head, last));
	return (ft_lstclear(&head, 1));
}
