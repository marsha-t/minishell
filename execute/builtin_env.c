/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:34:55 by mateo             #+#    #+#             */
/*   Updated: 2024/06/19 12:53:40 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	print_envp prints the environment variables 
	(prompted by env command)*/
void print_envp(t_var *envp, int out_fd)
{
	t_var *current;

	current = envp;
	while (current)
	{
		if (current->env == 1 && current->flag == 0)
		{
			write(out_fd, current->content, ft_strlen(current->content));
			write(out_fd, "\n", 1);
		}
		current = current -> next;
	}
}

/*	builtin_env runs the env command
	- if no args, prints environment variable list with flag = 0*/
int	builtin_env(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	(void)in_fd;
	(void)out_fd;
	if (node->n_args > 0)
		return (ft_putstr_fd("env: too many arguments/options\n", 2), 1);
	return (print_envp(shell->var_list, out_fd), 0);
}
