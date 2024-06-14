/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:34:55 by mateo             #+#    #+#             */
/*   Updated: 2024/06/05 06:41:14 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_env runs the env command
	- if no args, prints environment variable list with flag = 0*/
// work in progress: print_envp not finalised yet; needs to account for out_fd
int	builtin_env(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	(void)in_fd;
	(void)out_fd;
	if (node->n_args > 0)
		return (ft_putstr_fd("env: too many arguments/options\n", 2), 1);
	return (print_envp(shell->var_list), 0);
}
