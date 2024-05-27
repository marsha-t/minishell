/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/05/27 12:13:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
int	execute_cmd(t_ast *node, int in_fd, int out_fd)
{
	if (check_builtin(node->cmd) == 0)
		run_builtin(node, in_fd, out_fd);
	else
		run_external(node, in_fd, out_fd);
}

/*	execute_cmd_node
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
int	execute_cmd_node(t_ast *node)
{
	// int	exit_status;
	// handle redirections
	// exit_status = execute_cmd(node, );
	// return (exit_status);
}

/*	execute_ast traverses AST for execution
	- includes logic for TOKEN_AND and TOKEN_OR (which recursively calls execute_ast)
	*/
int	execute_ast(t_ast *node)
{
	if (!node)
		return (0);
	if (!node->left && !node->right) 
	{
		return (execute_cmd_node(node));
		return (0);
	}
	else if (node->code == TOKEN_PIPE)
	{
		// ???
	}
	else if (node->code == TOKEN_AND)
	{
		if (execute_ast(node->left) == 0)
			return (execute_ast(node->right));
		else
			return (1);
	}
	else if (node->code == TOKEN_OR)
	{
		if (execute_ast(node->left) == 0)
			return (0);
		else
			return (execute_ast(node->right));
	}
	return (0);
}
