/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:54:54 by mateo             #+#    #+#             */
/*   Updated: 2024/06/06 10:58:51 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_exit runs the exit command
	- frees everything allocated so far
	- clears history

	???
	- update SHLVL? */
int	builtin_exit(t_ast *node, int in_fd, int out_fd)
{
	if (node->n_args > 0 && node->args[0][0] == '-')
		return (ft_putstr_fd("exit: does not support options", 2), 1);
	free_shell(shell);
	rl_clear_history();
	return (0);
}