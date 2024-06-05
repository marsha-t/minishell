/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_modif.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/05 05:49:36 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int print_export(t_var **envp)
{
	t_var *current;

	current = *envp;
	while (current)
	{
		if (current->env == 1)
			ft_printf("declare -x %s\n", current->content);
		current = current -> next;
	}
	return (0);
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

// work in progress: to check return values of ft_printf
int print_envp(t_var **envp)
{
	t_var *current;

	current = *envp;
	while (current)
	{
		if (current->env == 1 && current->flag == 0)
			ft_printf("%s\n", current->content);
		current = current -> next;
	}
	return (0);
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
