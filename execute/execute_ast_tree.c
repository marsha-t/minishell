/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 18:06:46 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/*	execute_cmd checks whether command is built in and runs it if so
	otherwise, it searches for binary file for command */
// work in progress: check that exit_shell works properly
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
	// else if (ft_strchr(node->cmd, '=') != NULL)
	// 	exit_status = run_assign(node, shell);
	// else
	// 	exit_status = run_external(node, shell);
	if (shell->exit_shell == 1)
		return (exit_shell(shell, 1), 1);
	return(exit_status);
}
int	execute_cmd_others(t_ast *node, t_shell *shell)
{
	int	exit_status;

	exit_status = run_external(node, shell);
	if (shell->exit_shell == 1)
		return (exit_shell(shell, 1), 1);
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
	- if cmd is empty but there are args, first arg becomes cmd
	- for cases like: "" echo abc */
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
// work in progress: need to return from check_empty_cmd if empty str was typed
// work in progress: need to integrate quote removal
int	execute_cmd_node(t_ast *node, t_shell *shell)
{
	int id;
	if(get_docs(node) == 1)
		return(1);
	if (get_infile(node) == 1)
		return(1);
	if(get_outfile(node) == 1)
		return(1);
	if (check_var_expansion(node, shell) == 1)
		return (1);
	// 	if (check_wc_expansion(node) == 1)
	// 		return (1);
	if (cmd_only_quote(node->cmd) == 0)
	{
		shell->exit_status = 127;
		return (err_printf("minishell: : command not found\n"), shell->exit_status);
	}
	if (check_empty_cmd(node) == 1)
	{
		shell->exit_status = 0;
		return (0);
	}
	if (remove_quote_node(node) == 1)
		return (exit_shell(shell, 1), 1);
	if (ft_strcmp(node->cmd, "echo") == 0 || ft_strcmp(node->cmd, "cd") == 0 || ft_strcmp(node->cmd, "pwd") == 0 || ft_strcmp(node->cmd, "export") == 0 || ft_strcmp(node->cmd, "unset") == 0 || ft_strcmp(node->cmd, "env") == 0 || ft_strcmp(node->cmd, "exit") == 0)
		shell -> exit_status = execute_cmd_builtin(node, shell);
	else if (shell->pipe_data != 0)
	{
		LOC = 0;
		control_signals();
		shell -> exit_status = execute_cmd_others(node, shell);
	}
	else
	{
		id = fork();
		LOC = 0;
		if (id == 0)
		{
			control_signals();
			shell -> exit_status = execute_cmd_others(node, shell);
		}
		else
			waitpid(id, &shell -> exit_status, 0);
	}
	if (node->input_list && dup2(node ->tmp_stdin_fd , STDIN_FILENO)== -1)
			return(1);
	if (node->output_list && dup2(node ->tmp_stdout_fd , STDOUT_FILENO)== -1)
		return(1);
	return (shell -> exit_status);
}

int execute_pipe(t_ast *node, t_shell *shell, int flag)
{
	int	pipefd[2];
	pipe(pipefd);
	pid_t pid_left;
	pid_left = fork();
	if (pid_left == 0) // child process for left
	{
		if(flag == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		else if(flag == -1)
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
			dup2(pipefd[1],STDOUT_FILENO);
			close (pipefd[1]);
		}
		if (!node->left && !node->right)
			exit(execute_cmd_node(node, shell));
		else if (node->code == TOKEN_AND)
		{
			shell->pipe_data = 0;
			if (execute_ast(node->left, shell) == 0)
				exit (execute_ast(node->right, shell));
			else
				exit (1);
		}
		else if (node->code == TOKEN_OR)
		{
			if (execute_ast(node->left, shell) == 0)
				exit (0);
			else
				exit (execute_ast(node->right, shell));
		}
	}
	else
	{
		waitpid(pid_left, NULL, 0);
		shell->pipe_data = 1;
		if(shell ->old_read_fd != -2)
			close(shell ->old_read_fd);
		shell ->old_read_fd = pipefd[0];
		close(pipefd[1]);
	}
	return (0);
}

int	execute_pipeline(t_ast *node, t_shell *shell)
{
	int flag;
	int count;
	count = 0;
	flag = 0;
	while (node->pipe)
	{
		execute_pipe(node, shell,flag);
		node = node->pipe;
		flag++;
		count ++;
	}
	execute_pipe(node, shell,-1);
	close (shell -> old_read_fd);
	while (count)
	{
		wait(NULL);
		count--;
	}
	return (0);
}

/*	execute_ast traverses AST for execution
	- includes logic for TOKEN_AND and TOKEN_OR (which recursively calls execute_ast)
	*/
// work in progress: need to add code for pipe
int	execute_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->pipe)
	{
		shell->pipe_data = 1;
		execute_pipeline(node, shell);
	}
	else
	{
		shell->pipe_data = 0;
		if (!node->left && !node->right)
		{
			return (execute_cmd_node(node, shell));
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
	}
	return (0);
}

