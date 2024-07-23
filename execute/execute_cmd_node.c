/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:17:52 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 19:12:07 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_lists(t_ast *node, t_shell *shell)
{
	if (node->input_list && dup2(node->tmp_stdin_fd, STDIN_FILENO) == -1)
		return (1);
	if (node->output_list && dup2(node->tmp_stdout_fd, STDOUT_FILENO) == -1)
		return (1);
	if (node->heredoc_list && dup2(node->tmp_stdin_fd, STDIN_FILENO) == -1)
		return (1);
	if (node->tmp_stdin_fd != 0)
		close(node->tmp_stdin_fd);
	if (node->tmp_stdout_fd != 1)
		close(node->tmp_stdout_fd);
	return (g_loc = 1, shell->exit_status);
}

void	forking(t_shell *shell, t_ast *node)
{
	pid_t	id;
	int		status;

	id = fork();
	status = 0;
	g_loc = 0;
	if (id == 0)
	{
		control_signals();
		if (node->tmp_stdin_fd != 0)
			close(node->tmp_stdin_fd);
		if (node->tmp_stdout_fd != 1)
			close(node->tmp_stdout_fd);
		shell->exit_status = execute_cmd_others(node, shell);
	}
	else
	{
		waitpid(id, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}

void	create_pipe_background(t_shell *shell, t_ast *node)
{
	g_loc = 0;
	control_signals();
	shell->exit_status = execute_cmd_others(node, shell);
}

/*	execute_cmd_node
	- checks for empty cmd before - expands normal and environment variables
	- removes quotes
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
int	execute_cmd_node(t_ast *node, t_shell *shell)
{
	int	setup_return;

	setup_return = cmd_setup(node, shell);
	if (setup_return == 1)
	{
		shell->exit_status = 1;
		if (shell->exit_shell == 1)
			return (exit_shell(shell, 1), 1);
		else
			return (1);
	}
	else if (setup_return == 127 || setup_return == 2)
		return (shell->exit_status);
	if (check_builtin(node->cmd) == 0 || ft_strchr(node->cmd, '=') != NULL
		|| node->cmd[0] == '\0')
		shell->exit_status = execute_cmd_builtin(node, shell);
	else if (shell->pipe_data != 0)
		create_pipe_background(shell, node);
	else
		forking(shell, node);
	return (check_lists(node, shell));
}
