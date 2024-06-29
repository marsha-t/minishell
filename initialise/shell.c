/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:39:16 by mateo             #+#    #+#             */
/*   Updated: 2024/06/28 16:25:45 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_safe(void *pointer)
{
	free(pointer);
	pointer = 0;
}

/*	init_shell initialises t_shell
	- creates linked list of environment variables
	- initialises the rest to 0/NULL */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (ft_putstr_fd("Malloc error for shell structure\n", 2), NULL);
	shell->var_list = create_list(envp);
	// shell->directory_contents= create_conts_list();
	if (!shell->var_list)
		return (free_shell(shell), NULL);
	shell->line = 0;
	shell->tokens = 0;
	shell->root = 0;
	shell-> exit_status = -1;
	shell -> pipe_data = 0;
	return (shell);
}
void free_pipe(t_pipe_info *pipe)
{
	int k;
	k = 0;
	while (k < pipe ->pipe_count)
	{
		free(pipe->pipes[k]);
		k++;
	}
	free(pipe->pipes);
}

/*	free_shell frees the various structures within shell
	and then frees shell itself */
void	free_shell(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	if (shell->var_list)
		free_var_list(shell->var_list);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if(shell ->pipe_data)
		free_pipe(shell->pipe_data);
	free(shell);
	// ast_tree_free(shell->root);
}
