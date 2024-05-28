/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/05/28 15:40:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_builtin returns 1 if cmd is one of the required builtins
	otherwise, it returns 0*/
int	check_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 || \
		ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 || \
		ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 || \
		ft_strcmp(cmd, "exit") == 0)
		return (1)
	return (0);
}

/*	check_assign returns 1 if cmd is an assignment
	i.e. it contains unquoted = sign 
	- returns 0 otherwise */
int	check_assign(char *cmd)
{
	int		i;
	char	quote;
	
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == 34 || cmd[i] == 39)
		{
			if (quote == 0)
				quote = cmd[i];
			else if (quote != 0 && quote == cmd[i])
				quote = 0;
		}
		else if ((*cmd == "=") && quote == 0)
			return (1);
		i++;
	}
	return (0);
}

/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
int	execute_cmd(t_ast *node, int in_fd, int out_fd)
{
	if (check_builtin(node->cmd) == 1)
		run_builtin(node, in_fd, out_fd);
	else if (check_assign(node->cmd) == 1 && valid_varname(node->cmd) == 1)
		run_assign(node);
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
