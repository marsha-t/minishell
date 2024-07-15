/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:39:16 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 07:53:25 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	shlvl_increment adds one to SHLVL
	- if SHLVL doesn't exist, it creates it */
int	shlvl_increment(t_shell *shell)
{
	t_var	*shlvl_node;
	int		new_shlvl;

	shlvl_node = check_exist("SHLVL", shell->var_list);
	if (!shlvl_node)
		return (create_node(&shell->var_list, "SHLVL=1", 1));
	new_shlvl = ft_atoi(shlvl_node->value) + 1;
	free(shlvl_node->value);
	shlvl_node->value = ft_itoa(new_shlvl);
	if (!shlvl_node->value)
		return (err_printf("minishell: malloc error: ft_itoa\n"), 1);
	return (0);
}

/*	init_shell initialises t_shell
	- creates linked list of environment variables
	- initialises the rest to 0/NULL */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (err_printf("minishell: malloc error for shell structure\n"), NULL);
	shell->var_list = create_list(envp);
	if (!shell->var_list)
		return (free_shell(shell), NULL);
	shell->line = 0;
	shell->tokens = 0;
	shell->ast_list = 0;
	shell->root = 0;
	shell->exit_status = -1;
	shell->pipe_data = 0;
	shell->directory_contents = 0;
	shell->exit_shell = 0;
	if (shlvl_increment(shell) == 1)
		return (free_shell(shell), NULL);
	return (shell);
}

void	free_pipe(t_pipe_info *pipe)
{
	int	k;

	k = 0;
	while (k < pipe ->pipe_count)
	{
		free(pipe->pipes[k]);
		k++;
	}
	free(pipe->pipes);
}

/*	free_after_command frees data in shell
	 that need to be freed after every command */
void	free_after_command(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->pipe_data)
		free_pipe(shell->pipe_data);
	if (shell->ast_list)
		ast_list_free(shell->ast_list);
}

/*	free_shell frees everything in shell
	and then frees shell itself */
void	free_shell(t_shell *shell)
{
	free_after_command(shell);
	if (shell->var_list)
		free_var_list(shell->var_list);
	free(shell);
}
