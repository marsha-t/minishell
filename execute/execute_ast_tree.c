/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 14:31:23 by ryagoub          ###   ########.fr       */
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
	// if (shell->exit_shell == 1)
	// 	return (exit_shell(shell, 1), 1);
	return (exit_status);
}
/*	cmd_only_quote checks whether a commmand contains only consecutive empty quoted strings
	- return 0 if so
	- for cases like: "" echo abc */
int	cmd_only_quote(char *cmd)
{
	char quote;
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
		else if ((node->input_list) || (node->output_list) || (node->heredoc_list))
			return (0);
		else
			return (1);
	}
	else
		return (0);
}

int	cmd_setup(t_ast *node, t_shell *shell)
{
	int input_fd;
	int output_fd;
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
	if (get_docs(node, shell) == 1)
		return (1);
	input_fd =get_infile(node, shell);
	output_fd=get_outfile(node, shell);
	// printf("this is the input_fd %d \n",input_fd);
	// printf("this is the output_fd %d \n",output_fd);
	if (input_fd == 1 || output_fd== 1)
	{
		if (output_fd == 1 && input_fd != 1 && input_fd != 0)
			close(input_fd);
		return (1);
	}
	else
	{
		if(output_fd != 0)
			dup_output(shell, node, output_fd);
		if(input_fd!= 0)
			dup_input(shell, node, input_fd) ;
	}
	return (0);
}

/*	execute_cmd_node
	- checks for empty cmd before - expands normal and environment variables
	- removes quotes
	- sets up file descriptors given redirections
	- executes command with updated input/output fd */
// work in progress: update description of function
int	execute_cmd_node(t_ast *node, t_shell *shell)
{
	int id;
	int	status;
	int	setup_return;
  	// if (check_var_expansion(node, shell) == 1)
	// {
	// 	shell->exit_shell = 1;
	// 	return (exit_shell(shell, 1), 1);
	// }
	// if (check_wc_expansion(node, shell) == 1)
	// {
	// 	if (shell->exit_shell == 1)
	// 		return (exit_shell(shell, 1), 1);
	// 	else
	// 		return (1);
	// }
	// if (cmd_only_quote(node->cmd) == 0)
	// {
	// 	shell->exit_status = 127;
	// 	return (err_printf(": command not found\n"), shell->exit_status);
	// }
	// if (check_empty_cmd(node) == 1)
	// {
	// 	shell->exit_status = 0;
	// 	return (0);
	// }
	// if (remove_quote_node(node) == 1)
	// 	return (exit_shell(shell, 1), 1);
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
	{
		return (shell->exit_status);
	}
	// printf("this is the argument after expansion %s\n",(char*)node->args->content);
	if (check_builtin(node->cmd) == 0 || ft_strchr(node->cmd, '=') != NULL || node->cmd[0] == '\0')
		shell->exit_status = execute_cmd_builtin(node, shell);
	else if (shell->pipe_data != 0)
	{
		LOC = 0;
		control_signals();
		shell->exit_status = execute_cmd_others(node, shell);
	}
	else
	{
		id = fork();
		LOC = 0;
		if (id == 0)
		{
			control_signals();
			shell->exit_status = execute_cmd_others(node, shell);
		}
		else
		{
			waitpid(id, &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
		}
	}
	if (node->input_list && dup2(node->tmp_stdin_fd, STDIN_FILENO)== -1)
		return(1);
	if (node->output_list && dup2(node->tmp_stdout_fd, STDOUT_FILENO)== -1)
		return(1);
	if (node->heredoc_list && dup2(node->tmp_stdin_fd, STDIN_FILENO)== -1)
		return(1);
	return (LOC = 1, shell->exit_status);
}

// int execute_pipe(t_ast *node, t_shell *shell, int flag)
// {
// 	int	pipefd[2];
// 	// int	status;

// 	pipe(pipefd);
// 	pid_t pid_left;
// 	pid_left = fork();
// 	if (pid_left == 0) // child process for left
// 	{
// 		if(flag == 0)
// 		{
// 			close(pipefd[0]);
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[1]);
// 		}
// 		else if(flag == -1)
// 		{
// 			close(pipefd[0]);
// 			dup2(shell->old_read_fd, STDIN_FILENO);
// 			close(shell->old_read_fd);
// 			close(pipefd[1]);
// 		}
// 		else
// 		{
// 			close(pipefd[0]);
// 			dup2(shell->old_read_fd, STDIN_FILENO);
// 			close(shell->old_read_fd);
// 			dup2(pipefd[1],STDOUT_FILENO);
// 			close (pipefd[1]);
// 		}
// 		if (!node->left && !node->right)
// 			exit(execute_cmd_node(node, shell));
// 		else if (node->code == TOKEN_AND)
// 		{
// 			shell->pipe_data = 0;
// 			if (execute_ast(node->left, shell) == 0)
// 				exit (execute_ast(node->right, shell));
// 			else
// 				exit (1);
// 		}
// 		else if (node->code == TOKEN_OR)
// 		{
// 			if (execute_ast(node->left, shell) == 0)
// 				exit (0);
// 			else
// 				exit (execute_ast(node->right, shell));
// 		}
// 	}
// 	else
// 	{
// 		// waitpid(pid_left, &status, 0);
// 		shell->pipe_data = 1;
// 		if(shell->old_read_fd != -2)
// 			close(shell->old_read_fd);
// 		shell->old_read_fd = pipefd[0];
// 		close(pipefd[1]);
// 	}
// 	return (shell->exit_status);
// }

int execute_pipe(t_ast *node, t_shell *shell, int flag, int count) // MT: new code: added count
{
	int	pipefd[2];

	pipe(pipefd);
	shell->pid[count] = fork(); // MT: new code
	// pid_left = fork();
	if (shell->pid[count] == 0) // child process for left
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
		// waitpid(pid_left, &status, 0);
		shell->pipe_data = 1;
		if(shell->old_read_fd != -2)
			close(shell->old_read_fd);
		shell->old_read_fd = pipefd[0];
		close(pipefd[1]);
	}
	return (shell->exit_status);
}

int	init_pipeline(t_ast *node, t_shell *shell) // MT: new code: new function
{
	int	tot;
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
	int flag;
	int count;
	int status;

	flag = 0;
	count = 0;
	status = 0;

	// MT: new code: start
	if (init_pipeline(node, shell) == 1)
		return (1);
	while (node->pipe)
	{
		execute_pipe(node, shell,flag, count);
		node = node->pipe;
		flag++;
		count++;
	}
	execute_pipe(node, shell, -1, count);
	int i = 0;
	while (i <= count)
	{
		if (waitpid(shell->pid[i], &status, 0))
			if (WIFEXITED(status))
			{
				shell->exit_status = WEXITSTATUS(status);
			}
		i++;
	}
	// MT: new code: end

	// while (node->pipe)
	// {
	// 	execute_pipe(node, shell,flag);
	// 	node = node->pipe;
	// 	flag++;
	// 	count++;
	// }
	// count++;
	// execute_pipe(node, shell, -1);
	// count++;
	// while (count)
	// {
	// 	if (waitpid(shell->pid[count], &status, 0))
	// 		if (WIFEXITED(status))
	// 		{
	// 			shell->exit_status = WEXITSTATUS(status);
	// 		}
	// 	count --;
	// }

	free(shell->pid);
	close(shell->old_read_fd);
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
	if (node->pipe)
	{
		shell->pipe_data = 1;
		return(execute_pipeline(node, shell));
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

