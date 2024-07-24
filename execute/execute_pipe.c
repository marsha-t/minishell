/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:24:38 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/24 18:00:47 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	piping(int	*pipefd, t_shell *shell, int flag)
{
	if (flag == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	else if (flag == -1)
	{
		close(pipefd[0]);
		dup2(shell->old_read_fd, STDIN_FILENO);
		close(shell->old_read_fd);
		close(pipefd[1]);
	}
	else
	{
		close(pipefd[0]);
		dup2(shell->old_read_fd, STDIN_FILENO);
		close(shell->old_read_fd);
		dup2(pipefd[1], STDOUT_FILENO);
		close (pipefd[1]);
	}
}

void	executing(t_shell *shell, t_ast *node)
{
	if (!node->left && !node->right)
	{
		shell->exit_status = execute_cmd_node(node, shell);
		exit_shell(shell, shell->exit_status);
	}
	else if (node->code == TOKEN_AND)
	{
		shell->pipe_data = 0;
		if (execute_ast(node->left, shell) == 0)
		{
			shell->exit_status = execute_ast(node->right, shell);
			exit_shell(shell, shell->exit_status);
		}
		else
			exit_shell (shell, 1);
	}
	else if (node->code == TOKEN_OR)
	{
		if (execute_ast(node->left, shell) == 0)
			exit_shell (shell, 0);
		else
			exit_exit_status_modif(node, shell);
	}
}

int	execute_pipe(t_ast *node, t_shell *shell, int flag, int count)
{
	int	pipefd[2];

	if (count == 0)
		flag = 0;
	pipe(pipefd);
	shell->pid[count] = fork();
	if (shell->pid[count] == 0)
	{
		piping(pipefd, shell, flag);
		executing(shell, node);
	}
	else
	{
		shell->pipe_data = 1;
		if (shell->old_read_fd != -2)
			close(shell->old_read_fd);
		shell->old_read_fd = pipefd[0];
		close(pipefd[1]);
	}
	return (shell->exit_status);
}

int	init_pipeline(t_ast *node, t_shell *shell)
{
	int		tot;
	t_ast	*curr;

	curr = node->pipe;
	tot = 1;
	while (curr)
	{
		tot++;
		curr = curr->pipe;
	}
	shell->pid = malloc(sizeof(pid_t) * tot);
	if (!shell->pid)
		return (1);
	return (0);
}

int	execute_pipeline(t_ast *node, t_shell *shell)
{
	int	count;
	int	status;
	int	i;

	count = 0;
	status = 0;
	i = 0;
	if (init_pipeline(node, shell) == 1)
		return (1);
	while (node->pipe)
	{
		execute_pipe(node, shell, 2, count);
		node = node->pipe;
		count++;
	}
	execute_pipe(node, shell, -1, count);
	while (i <= count)
	{
		if (waitpid(shell->pid[i], &status, 0))
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
		i++;
	}
	return (free(shell->pid), close(shell->old_read_fd), shell->exit_status);
}
