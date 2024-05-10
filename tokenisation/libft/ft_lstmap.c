/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:34:38 by mateo             #+#    #+#             */
/*   Updated: 2024/01/03 23:34:38 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*next;
	t_list	*start;
	void	*temp_content;

	if (!lst)
		return (0);
	start = 0;
	while (lst)
	{
		temp_content = f(lst->content);
		next = ft_lstnew(temp_content);
		if (!next)
		{
			ft_lstclear(&start, (*del));
			del(temp_content);
			return (0);
		}
		ft_lstadd_back(&start, next);
		lst = lst->next;
	}
	return (start);
}
