/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:35:36 by mateo             #+#    #+#             */
/*   Updated: 2024/07/20 14:59:32 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_cd runs the cd command
	- if no arg, sets to $HOME
	- error that do not terminate shell:
		- no arg given but HOME is unset
		- more than 1 argument
		- file given instead of directory
		- no permission given for directory
		- file doesn't exist
		- file is too long
	- chdir will work with relative and absolute path */
int	builtin_cd(t_ast *node, t_shell *shell)
{
	int			error;
	struct stat	file_stat;
	char		*path;

	if (node->n_args == 0)
	{
		path = expand_var("HOME", shell->var_list);
		if (path[0] == '\0')
			return (err_printf("cd: HOME not set"), 1);
		else if (!path)
		{
			shell->exit_shell = 1;
			err_printf("malloc error: expand_var in builtin_cd\n");
			return (1);
		}
	}
	else if (node->n_args > 1)
		return (err_printf("cd: too many arguments\n"), 1);
	else
	{
		path = ft_strdup(node->args->content);
		if (!path)
		{
			shell->exit_shell = 1;
			err_printf("malloc error: path in builtin_cd\n");
			return (1);
		}
	}
	error = access(path, F_OK);
	if (error == 0)
	{
		if (stat(path, &file_stat) == -1)
		{
			shell->exit_shell = 1;
			free(path);
			return (err_printf("cd: error calling stat\n"), 1);
		}
		if (!S_ISDIR(file_stat.st_mode))
		{
			err_printf("cd: %s: Not a directory\n", path);
			free(path);
			return (1);
		}
		else
		{
			if (chdir(path) == 0)
				return (free(path), 0);
			else
			{
				shell->exit_shell = 1;
				err_printf("cd: error calling chdir\n");
				free(path);
				return (1);
			}
		}
	}
	else if (errno == EACCES)
	{
		err_printf("%s: Permission denied\n", path);
		free(path);
		return (126);
	}
	else if (errno == ENAMETOOLONG)
	{
		err_printf("cd: %s: File name too long\n", path);
		free(path);
		return (126);
	}
	else if (errno == ENOENT)
	{
		free(path);
		return (err_printf(" No such file or directory\n"), 1);
	}
	else if (errno == ENOTDIR)
	{
		err_printf("%s: Not a directory\n", path);
		free(path);
		return (1);
	}
	else
	{
		free(path);
		shell->exit_shell = 1;
		return (err_printf("cd: error calling access\n"), 1);
	}
	return (0);
}
