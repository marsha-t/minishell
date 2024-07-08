/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:35:36 by mateo             #+#    #+#             */
/*   Updated: 2024/07/07 16:34:25 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_cd runs the cd command
	- if no arg, sets to $HOME
	- error if 
		- more than 1 argument
		- file given instead of directory
		- no permission given for directory
	- chdir will work with relative and absolute path */
/* work in progress: 
	work in progress: catered for other errno as error calling access - this should terminate shell?
	work in progress: need to free path when node->n_args == 0; 
		else expand_var to not strdup
*/
int	builtin_cd(t_ast *node, t_shell *shell)
{
	int	error;
	struct stat	file_stat;
	char *path;
	
	if (node->n_args == 0)
		path = expand_var("HOME", shell->var_list);
	else if (node->n_args > 1)
		return (err_printf("minishell: cd: too many arguments\n"), 1);
	else
		path = node->args->content;

	error = access(path, F_OK);
	if (error == 0)
	{
		if (stat(path, &file_stat) == -1)
		{
			shell->exit_shell = 1;
			return (err_printf("minishell: cd: error calling stat\n", 2), 1);
		}
		if (!S_ISDIR(file_stat.st_mode))
		{
			err_printf("minishell: cd: %s: Not a directory\n", path);
			return (1);
		}
		else
		{
			if (chdir(path) == 0)
				return (0);
			else
			{
				shell->exit_shell = 1;
				return (err_printf("minishell: cd: error calling chdir\n"), 1);			
			}
		}
	}
	else if (errno == EACCES)
		return (ft_putstr_fd("minishell: Permission denied\n", 2), 126);
	else if (errno == ENAMETOOLONG)
		return (err_printf("minishell: cd: %s: File name too long\n", path), 126);
	else if (errno == ENOENT)
		return (err_printf("minishell: cd: %s: No such file or directory\n", path), 127);
	else
	{
		shell->exit_shell = 1;
		return (err_printf("minishell: cd: error calling access\n"), 1);
	}
	return (0);
}
