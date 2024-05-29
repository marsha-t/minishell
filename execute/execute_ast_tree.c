/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/05/29 08:39:41 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
// work in progress - whether to give in_fd and out_fd to run_assign
int	execute_cmd(t_ast *node, int in_fd, int out_fd)
{
	int	exit_status;
	
	if (ft_strcmp(cmd, "echo") == 0)
		exit_status = builtin_echo(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "cd") == 0)
		exit_status = builtin_cd(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_status = builtin_pwd(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_status = builtin_export(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_status = builtin_unset(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_status = builtin_env(node, in_fd, out_fd);
	else if (ft_strcmp(cmd, "exit") == 0)
		exit_status = builtin_exit(node, in_fd, out_fd);
	else if (ft_strchr(node->cmd, '=') != NULL)
		exit_status = run_assign(node);
	else
		exit_status = run_external(node, in_fd, out_fd);
	return(exit_status);
}

/*	execute_cmd_node
	- expands normal and environment variables
	- removes quotes
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
// work in progress
int	execute_cmd_node(t_ast *node)
{
	// int	exit_status;
	// expansions
	if (remove_quote_node(node) == 1)
		return (1);
	// handle redirections
	// exit_status = execute_cmd(node, );
	// return (exit_status);
}

/*	execute_ast traverses AST for execution
	- includes logic for TOKEN_AND and TOKEN_OR (which recursively calls execute_ast)
	*/
// work in progress for pipe
int	execute_ast(t_ast *node)
{
	if (!node)
		return (0);
	if (!node->left && !node->right) 
		return (execute_cmd_node(node));
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
