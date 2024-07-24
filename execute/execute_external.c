/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:19:03 by mateo             #+#    #+#             */
/*   Updated: 2024/07/24 16:44:01 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	count_env counts number of env var in linked list */
int	count_env(t_var *env)
{
	int		count;
	t_var	*current;

	count = 0;
	current = env;
	while (current)
	{
		if (current->env == 1)
			count++;
		current = current->next;
	}
	return (count);
}

/*	envp_array converts env var
linked list into a null-terminated array for execve
	- includes all variables listed in export (even if they are not in env)*/
char	**envp_array(t_var *env)
{
	char	**envp;
	t_var	*current;
	int		i;

	envp = malloc(sizeof(char *) * (count_env(env) + 1));
	if (!envp)
		return (err_printf("malloc error: envp for envp_array\n"), NULL);
	i = 0;
	current = env;
	while (current)
	{
		if (current->env == 1)
		{
			envp[i] = ft_strjoin_free(ft_strjoin_free(ft_strdup(current->key),
						ft_strdup("=")), ft_strdup(current->value));
			if (!envp[i])
				return (free_array(envp, i),
					err_printf("malloc error:e_array\n"), NULL);
			i++;
		}
		current = current->next;
	}
	return (envp[i] = NULL, envp);
}

/*	argv_array converts cmd and arg
in node into null-terminated array for execve*/
char	**argv_array(t_ast *node)
{
	char	**argv;
	t_list	*current;
	int		i;

	current = node->args;
	argv = malloc(sizeof(char *) * (ft_lstsize(current) + 2));
	if (!argv)
		return (err_printf("malloc error: argv for argv_array\n"), NULL);
	argv[0] = ft_strdup(node->cmd);
	if (!argv[0])
		return (free(argv), err_printf("malloc error: a_array\n"), NULL);
	i = 1;
	current = node->args;
	while (current)
	{
		argv[i] = ft_strdup(current->content);
		if (!argv[i])
			return (free_array(argv, i),
				err_printf("malloc error: argv[i] for argv_array\n"), NULL);
		i++;
		current = current->next;
	}
	return (argv[i] = NULL, argv);
}

/*	get_filepath extracts the correct filepath for execve
	- checks whether cmd is a specified path or a cmd
		- specified path will start with '.' or '/'
	- if specified path, call check_filepath() to check the file
	- if cmd, call find_cmd() to find filepath */
char	*get_filepath(char *cmd, int *exit_status, t_shell *shell)
{
	char	*filepath;

	if (ft_strchr(cmd, '/') != 0)
	{
		*exit_status = check_filepath(cmd, shell);
		if (*exit_status == 0)
		{
			filepath = ft_strdup(cmd);
			if (!filepath)
			{
				*exit_status = 1;
				shell->exit_shell = 1;
				return (err_printf("malloc error: filepath in get_filepath\n"),
					NULL);
			}
		}
		else
			filepath = 0;
	}
	else
		filepath = find_cmd(cmd, exit_status, shell);
	return (filepath);
}

/*	run_external runs an external command
	- by first preparing the arguments for execve */
int	run_external(t_ast *node, t_shell *shell)
{
	int		exit_status;
	char	*filename;
	char	**argv;
	char	**envp;

	filename = get_filepath(node->cmd, &exit_status, shell);
	if (!filename)
		exit_shell (shell, exit_status);
	argv = argv_array(node);
	if (!argv)
		exit_shell(shell, 1);
	envp = envp_array(shell->var_list);
	if (!envp)
		exit_shell(shell, 1);
	exit_status = execve(filename, argv, envp);
	free_char_dp(argv);
	free_char_dp(envp);
	free(filename);
	if (exit_status == -1 && errno == EACCES)
		exit_status = 126;
	err_printf("%s\n", strerror(errno));
	exit_shell (shell, exit_status);
	return (exit_status);
}
