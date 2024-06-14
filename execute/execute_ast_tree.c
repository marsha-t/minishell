/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/06/14 15:58:21 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
// work in progress - whether to give in_fd and out_fd to run_assign
int	execute_cmd(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	int	exit_status;
	
	if (ft_strcmp(node->cmd, "echo") == 0)
		exit_status = builtin_echo(node, in_fd, out_fd);
	// else if (ft_strcmp(node->cmd, "cd") == 0)
	// 	exit_status = builtin_cd(node, in_fd, out_fd);
	else if (ft_strcmp(node->cmd, "pwd") == 0)
		exit_status = builtin_pwd(node, in_fd, out_fd);
	// else if (ft_strcmp(node->cmd, "export") == 0)
	// 	exit_status = builtin_export(node, in_fd, out_fd, shell);
	// else if (ft_strcmp(node->cmd, "unset") == 0)
	// 	exit_status = builtin_unset(node, in_fd, out_fd, shell);
	// else if (ft_strcmp(node->cmd, "env") == 0)
	// 	exit_status = builtin_env(node, in_fd, out_fd, shell);
	else if (ft_strcmp(node->cmd, "exit") == 0)
		exit_status = builtin_exit(node, shell);
	// else if (ft_strchr(node->cmd, '=') != NULL)
	// 	exit_status = run_assign(node, shell);
	else
		exit_status = run_external(node, in_fd, out_fd, shell);
	return(exit_status);
}

/*	cmd_only_quote checks whether a commmand contains only consecutive empty quoted strings
	- return 0 if so */
int	cmd_only_quote(char *cmd)
{
	char quote;
	
	quote = 0;
	while (*cmd == 34 || *cmd == 39)
	{
		if (quote == 0)
			quote = *cmd;
		else
			quote = quote - *cmd;
		if (quote != 0 && quote != 34 && quote != 39)
			return (1);
		cmd++;
	}
	if (*cmd == '\0')
		return (0);
	else
		return (1);
}
/*	check_empty_cmd checks whether cmd is an empty str
	- returns 0 if 
		- cmd is not empty
		- cmd is empty but there are args
	- returns 1 if cmd is empty and there are no args
	- if cmd is empty but there are args, first arg becomes cmd */
int	check_empty_cmd(t_ast *node)
{
	t_list	*curr_arg;

	if (node->cmd[0] == '\0')
	{
		if (node->n_args == 0)
			return (1);
		else
		{
			curr_arg = node->args;
			node->args = node->args->next;
			node->cmd = curr_arg->content;
			free(curr_arg);
			node->n_args--;
			return (0);
		}
	}
	else
		return (0);
}

/*	execute_cmd_node
	- checks for empty cmd before - expands normal and environment variables
	- removes quotes
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
// work in progress: need to integrate variable expansion
// work in progress: need to integrate wildcard expansion
// work in progress: need to return from check_empty_cmd if empty str was typed
// work in progress: need to integrate quote removal
// work in progress: need to add redirections
int	execute_cmd_node(t_ast *node, t_shell *shell)
{
	int	exit_status;
	int in_fd;
	int out_fd;

	// variable expansion
	// wildcard expansion
	if (cmd_only_quote(node->cmd) == 0)
		return (ft_putstr_fd("command not found\n", 2), 127);
	if (check_empty_cmd(node) == 1)
		return (1); // need to treat as if empty string was typed 
	// if (remove_quote_node(node) == 1)
	// 	return (1);
	// handle redirections
	in_fd = 0;
	out_fd = 1;
	exit_status = execute_cmd(node, in_fd, out_fd, shell);
	return (exit_status);
}

/*	execute_ast traverses AST for execution
	- includes logic for TOKEN_AND and TOKEN_OR (which recursively calls execute_ast)
	*/
// work in progress: need to add code for pipe
int	execute_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (!node->left && !node->right) 
		return (execute_cmd_node(node, shell));
	else if (node->code == TOKEN_PIPE)
	{
		// ???
	}
	else if (node->code == TOKEN_AND)
	{
		if (execute_ast(node->left, shell) == 0)
			return (execute_ast(node->right, shell));
		else
			return (1);
	}
	else if (node->code == TOKEN_OR)
	{
		if (execute_ast(node->left, shell) == 0)
			return (0);
		else
			return (execute_ast(node->right, shell));
	}
	return (0);
}
