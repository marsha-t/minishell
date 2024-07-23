/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 18:51:22 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_builtin returns 0 if cmd is a builtin command
	- returns 1 otherwise */
int	check_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	else
		return (1);
}

/*execute_cmd_builtin runs builtins and assignments
	-  calls exit_shell if builtin or assignment has exit_shell error
	*/
int	execute_cmd_builtin(t_ast *node, t_shell *shell)
{
	int	exit_status;

	if (ft_strcmp(node->cmd, "echo") == 0)
		exit_status = builtin_echo(node);
	else if (ft_strcmp(node->cmd, "cd") == 0)
		exit_status = builtin_cd(node, shell);
	else if (ft_strcmp(node->cmd, "pwd") == 0)
		exit_status = builtin_pwd(node, shell);
	else if (ft_strcmp(node->cmd, "export") == 0)
		exit_status = builtin_export(node, shell);
	else if (ft_strcmp(node->cmd, "unset") == 0)
		exit_status = builtin_unset(node, shell);
	else if (ft_strcmp(node->cmd, "env") == 0)
		exit_status = builtin_env(node, shell);
	else if (ft_strcmp(node->cmd, "exit") == 0)
		exit_status = builtin_exit(node, shell);
	else if (ft_strchr(node->cmd, '=') != NULL)
		exit_status = run_assign(node, shell);
	else if (node->cmd[0] == '\0')
		return (0);
	if (shell->exit_shell == 1)
		return (exit_shell(shell, 1), 1);
	return (exit_status);
}

// work in progress: update description of function
int	execute_cmd_others(t_ast *node, t_shell *shell)
{
	int	exit_status;

	exit_status = run_external(node, shell);
	return (exit_status);
}

/*	cmd_only_quote checks whether
a commmand contains only consecutive empty quoted strings
	- return 0 if so
	- for cases like: "" echo abc */
int	cmd_only_quote(char *cmd)
{
	char	quote;
	char	*start;

	start = cmd;
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
	if (*cmd == '\0' && start != cmd)
		return (0);
	else
		return (1);
}

// work in progress: update description of function
/*
	execute_ast traverses AST for execution
	- includes logic for TOKEN_AND and TOKEN_OR
	(which recursively calls execute_ast)
	*/
// work in progress: need to add code for pipe
int	execute_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->pipe)
		return (shell->pipe_data = 1, execute_pipeline(node, shell));
	else
	{
		shell->pipe_data = 0;
		if (!node->left && !node->right)
			return (execute_cmd_node(node, shell));
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
	}
	return (0);
}
