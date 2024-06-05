/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 06:17:45 by mateo             #+#    #+#             */
/*   Updated: 2024/06/03 06:17:52 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void unset(char *s1,char *s2, t_var **envp)
{
	t_var *current;
	current = *envp;
	t_var *del_node;
	ft_printf("%d \n",ft_strcmp(s1, "unset"));
	del_node = malloc(sizeof(t_var));
	if(ft_strcmp(s1, "unset")== 0)
	{
		if(search_for_node(s2, envp) )
			del_node = search_for_node(s2, envp);
		else
			return ;
	}
	if (del_node == *envp)
		return((*envp)= (*envp)->next ,free(del_node)) ;

	while(current->next)
	{
		if(current-> next== del_node)
		{
			current ->next = del_node ->next;
			free(del_node);
			return ;
		}
		current = current-> next;
	}
}
