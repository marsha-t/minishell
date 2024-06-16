/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_modif.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/07 05:28:42 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_export(t_var *envp)
{
	t_var *current;

	current = envp;
	while (current)
	{
		if (current->env == 1)
			ft_printf("declare -x %s\n", current->content);
		current = current -> next;
	}
}

// void print_export(char *str, t_var **envp)
// {
// 	t_var *current;

// 	current = *envp;
// 	if(ft_strcmp(str, "export") == 0 )
// 	{
// 		while (current)
// 		{
// 			ft_printf("declare -x %s\n", current->content);
// 			current = current -> next;
// 		}
// 	}
// }

void print_envp(t_var *envp)
{
	t_var *current;

	current = envp;
	while (current)
	{
		if (current->env == 1 && current->flag == 0)
			ft_printf("%s\n", current->content);
		current = current -> next;
	}
}

// void print_envp(char *str, t_var **envp)
// {
// 	t_var *current;

// 	current = *envp;
// 	if(ft_strcmp(str, "env") == 0 )
// 	{
// 		while (current)
// 		{
// 			if (current -> flag == 0)
// 				ft_printf("%s\n", current->content);
// 			current = current -> next;
// 		}
// 	}
// }
