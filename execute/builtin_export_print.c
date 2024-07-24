/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:39:26 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 08:40:21 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	print_export prints the environment variables
	(prompted by export command)*/
void	print_export(t_var *envp)
{
	t_var	*current;

	current = envp;
	while (current)
	{
		if (current->env == 1)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, current->key, ft_strlen(current->key));
			if (current->value)
			{
				write(STDOUT_FILENO, "=\"", 2);
				write(STDOUT_FILENO, current->value, ft_strlen(current->value));
				write(STDOUT_FILENO, "\"", 1);
			}
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}
