/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_modif.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 15:47:56 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void print_export(char *str, t_var **envp)
{
	t_var *current;

	current = *envp;
	if(ft_strcmp(str, "export") == 0 )
	{
		while (current)
		{
			ft_printf("declare -x %s\n", current->content);
			current = current -> next;
		}
	}
}

void print_envp(char *str, t_var **envp)
{
	t_var *current;

	current = *envp;
	if(ft_strcmp(str, "env") == 0 )
	{
		while (current)
		{
			if (current -> flag == 0)
				ft_printf("%s\n", current->content);
			current = current -> next;
		}
	}
}

void export(char *s1,char *s2, t_var **envp)
{
	t_var *current;
	// t_var *p;
	t_var *new;

	current = *envp;
	new = malloc(sizeof(t_var));
	if(ft_strcmp(s1, "export") == 0 && s2 )
	{
		if(check_exist(s2,*envp))
		{
			new= check_exist(s2, *envp);
			new ->content = s2;
		}
		else
		{
			*envp = new;
			new -> next = current;
			new ->content = s2;
			if(!(ft_strchr(s2,'=')))
				new-> flag = 1;
		}
	}
}


void unset(char *s1,char *s2, t_var **envp)
{
	t_var *current;
	current = *envp;
	t_var *del_node;
	printf("%d \n",ft_strcmp(s1, "unset"));
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
