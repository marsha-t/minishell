/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:19:03 by mateo             #+#    #+#             */
/*   Updated: 2024/06/28 01:03:01 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	count_env counts number of env var in linked list */
int	count_env(t_var *env)
{
	int	count;
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

/*	envp_array converts env var linked list into a null-terminated array for execve
	- includes all variables listed in export (even if they are not in env)*/
char	**envp_array(t_var *env)
{
	char	**envp;
	t_var	*current;
	int	i;

	envp = malloc(sizeof(char *) * (count_env(env) + 1));
	if (!envp)
		return (ft_putstr_fd("Malloc error creating envp for envp_array\n", 2), NULL);
	i = 0;
	current = env;
	while (current)
	{
		if (current->env == 1)
		{
			envp[i] = ft_strjoin(ft_strjoin(current->key, "="), current->value);
			if (!envp[i])
			{
				while (i--)
					free(envp[i]);
				free(envp);
				return (ft_putstr_fd("Malloc error creating envp[i] for envp_array\n", 2), NULL);
			}
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

/*	argv_array converts cmd and arg in node into null-terminated array for execve*/
char **argv_array(t_ast *node)
{
	char	**argv;
	t_list	*current;
	int		i;

	current = node->args;
	argv = malloc(sizeof(char *) * (ft_lstsize(current) + 2));
	if (!argv)
		return (ft_putstr_fd("Malloc error creating argv for argv_array\n", 2), NULL);
	argv[0] = ft_strdup(node->cmd);
	if (!argv[0])
	{
		free(argv);
		return (ft_putstr_fd("Malloc error creating argv[i] for argv_array\n", 2), NULL);
	}
	i = 1;
	current = node->args;
	while (current)
	{
		argv[i] = ft_strdup(current->content);
		if (!argv[i])
		{
			while (i--)
				free(argv[i]);
			free(argv);
			return (ft_putstr_fd("Malloc error creating argv[i] for argv_array\n", 2), NULL);
		}
		i++;
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

/*	get_filepath extracts the correct filepath for execve
	- checks whether cmd is a specified path or a cmd
		- error if cmd is empty str
		- specified path will start with '.' or '/'
	- if specified path, call check_filepath() to check the file
	- if cmd, call find_cmd() to find filepath */
char *get_filepath(char *cmd, int *exit_status, t_shell *shell)
{
	char	*filepath;

	if (cmd[0] == '\0')
	{
		*exit_status = 127;
		return (ft_putstr_fd("command not found\n", 2), NULL);
	}
	if (ft_strchr(cmd, '/') != 0)
	{
		*exit_status = check_filepath(cmd, shell);
		if (*exit_status == 0)
			filepath = cmd;
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
		return (exit_status);
	argv = argv_array(node);
	if (!argv)
		return (1);
	envp = envp_array(shell->var_list);
	if (!envp)
		return (1);
	exit_status = execve(filename, argv, envp);
	free_char_dp(argv);
	free_char_dp(envp);
	free(filename);
	return (exit_status);
}
