/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:34:55 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 21:35:44 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	print_envp prints the environment variables
	(prompted by env command)*/
void	print_envp(t_var *envp)
{
	t_var	*current;

	current = envp;
	while (current)
	{
		if (current->env == 1 && current->flag == 1)
		{
			write(STDOUT_FILENO, current->key, ft_strlen(current->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, current->value, ft_strlen(current->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current -> next;
	}
}

/*	builtin_env runs the env command
	- error if there are args
	- otherwise, prints env var list with flag = 0*/
int	builtin_env(t_ast *node, t_shell *shell)
{
	if (node->n_args > 0)
		return (err_printf("env: too many arguments/options\n"), 1);
	return (print_envp(shell->var_list), 0);
}
