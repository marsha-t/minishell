/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/06/27 23:59:04 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
// work in progress: check that exit_shell works properly  
int	execute_cmd(t_ast *node, t_shell *shell)
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
	// else if (ft_strchr(node->cmd, '=') != NULL)
	// 	exit_status = run_assign(node, shell);
	else
		exit_status = run_external(node, shell);
	if (shell->exit_shell == 1)
		return (exit_shell(shell), 1);
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

/*	file_list_check_var calls contain_var and expand_str
	for each file provided in file linked list
	*/
int	file_list_check_var(t_file *file, t_shell *shell)
{
	t_file	*curr_file;

	curr_file = file;
	while (curr_file)
	{
		if (contain_var(curr_file->file_name) == 0)
		{
			curr_file->file_name = expand_str(curr_file->file_name, shell->var_list);
			if (!curr_file->file_name)
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}

/*	check_var_expansion checks whether variable expansions are needed
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion
	*/
int	check_var_expansion(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	if (contain_var(node->cmd) == 0)
	{
		node->cmd = expand_str(node->cmd, shell->var_list);
		if (!node->cmd)
			return (1);
	}
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (contain_var(curr_arg->content) == 0)
			{
				curr_arg->content = expand_str(curr_arg->content, shell->var_list);
				if (!curr_arg->content)
					return (1);
			}
			curr_arg = curr_arg->next;
		}
	}
	if (node->input_list)
	{
		if (file_list_check_var(node->input_list, shell) == 1)
			return (1);
	}
	if (node->heredoc_list)
	{
		if (file_list_check_var(node->heredoc_list, shell) == 1)
			return (1);
	}
	if (node->output_list)
	{
		if (file_list_check_var(node->output_list, shell) == 1)
			return (1);
	}
	return (0);
}

/*	check_wc_expansion checks whether wildcard expansions are needed
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion
	*/
// work in progress: finish function - same as check_var_expansion but with contain_wc and corresponding expand function

// int	check_wc_expansion(t_ast *node)
// {
// }

/*	execute_cmd_node
	- checks for empty cmd before - expands normal and environment variables
	- removes quotes
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
// work in progress: need to return from check_empty_cmd if empty str was typed
// work in progress: need to integrate quote removal
int	execute_cmd_node(t_ast *node, t_shell *shell)
{
	// int id;
	// int status;

	// status =0;
	// id = fork();
	if (check_var_expansion(node, shell) == 1)
		return (1);
	// if (check_wc_expansion(node) == 1)
	// 	return (1);
	// if (id == 0)
	// {
		// if (get_infile(node) == 1)
		// 	return(1);
		// if(get_outfile(node) == 1)
		// 	return(1);
		if (cmd_only_quote(node->cmd) == 0)
			return (ft_putstr_fd("command not found\n", 2), 127);
		if (check_empty_cmd(node) == 1)
			return (1); // need to treat as if empty string was typed
	if (remove_quote_node(node) == 1)
		return (1);
	// handle redirections
		shell -> exit_status = execute_cmd(node, shell);
		// if (dup2(node ->tmp_stdin_fd , STDIN_FILENO)== -1)
		// 	return(1);
		// if (dup2(node ->tmp_stdout_fd , STDOUT_FILENO)== -1)
		// 	return(1);
		// close_files(node);
		// exit (shell->exit_status);
	// }
	// else
	// 	waitpid(id, &status, 0);
	return (shell->exit_status);
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
