/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:38:38 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 18:52:30 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_after_error(int input_fd, int output_fd, int here_doc_fd)
{
	if (input_fd == 1)
	{
		if (here_doc_fd != 1 && here_doc_fd != 0)
		{
			close(here_doc_fd);
			unlink("X9f4Tp1");
		}
	}
	if (output_fd == 1)
	{
		if (input_fd != 1 && input_fd != 0)
			close(input_fd);
		if (here_doc_fd != 1 && here_doc_fd != 0)
		{
			close(here_doc_fd);
			unlink("X9f4Tp1");
		}
	}
}

int	redir_setup(t_ast *node, t_shell *shell)
{
	int	input_fd;
	int	output_fd;
	int	here_doc_fd;

	here_doc_fd = get_docs(node, shell);
	input_fd = get_infile(node, shell);
	output_fd = get_outfile(node, shell);
	if (input_fd == 1 || output_fd == 1 || here_doc_fd == 1)
	{
		close_after_error(input_fd, output_fd, here_doc_fd);
		return (1);
	}
	else
	{
		if (output_fd != 0)
			dup_output(node, output_fd);
		if (input_fd != 0)
			dup_input(shell, node, input_fd);
		if (here_doc_fd != 0)
			dup_heredoc(node, shell, here_doc_fd);
	}
	return (0);
}

int	cmd_setup(t_ast *node, t_shell *shell)
{
	if (check_var_expansion(node, shell) == 1)
		return (1);
	if (check_wc_expansion(node, shell) == 1)
		return (1);
	if (cmd_only_quote(node->cmd) == 0)
	{
		shell->exit_status = 127;
		return (err_printf(": command not found\n"), shell->exit_status);
	}
	if (check_empty_cmd(node) == 1)
	{
		shell->exit_status = 0;
		return (2);
	}
	if (remove_quote_node(node) == 1)
		return (1);
	if (redir_setup(node, shell) == 1)
		return (1);
	return (0);
}

/*	check_empty_cmd checks whether cmd is an empty str
	- returns 0 if
		- cmd is not empty
		- cmd is empty but there are args
	- returns 1 if cmd is empty and there are no args
	- if cmd is empty but there are args, first arg becomes cmd
	- case: $var echo abc
	*/
int	check_empty_cmd(t_ast *node)
{
	t_list	*curr_arg;

	if (node->cmd[0] == '\0')
	{
		if (node->n_args > 0)
		{
			curr_arg = node->args;
			node->args = node->args->next;
			free(node->cmd);
			node->cmd = curr_arg->content;
			free(curr_arg);
			node->n_args--;
			return (0);
		}
		else if ((node->input_list) || (node->output_list)
			|| (node->heredoc_list))
			return (0);
		else
			return (1);
	}
	else
		return (0);
}
