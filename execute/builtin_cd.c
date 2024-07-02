/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:35:36 by mateo             #+#    #+#             */
/*   Updated: 2024/06/28 00:00:11 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_cd runs the cd command
	- if no arg, sets to $HOME*/
/* work in progress: 
	test: 	cd with no args;
			cd with more than 1 arg;
			cd to a file (exit number)
			cd to permission denied
			cd to long filename
			cd to nonexist file or directory
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
	else
		path = node->args->content;
	error = access(path, F_OK);
	if (error = 0)
	{
		if (stat(cmd, &file_stat) == -1)
		{
			shell->exit_shell = 1;
			return (ft_putstr_fd("Error calling stat\n", 2), 1); // note: this should terminate shell
		}
		if (!S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("Error?"); // cd given path to file instead of directory
			return (1);
		}
		else
		{
			if (chdir(path) == 0)
				return (0);
			else
			{
				shell->exit_shell = 1;
				return (ft_putstr_fd("Error calling chdir\n", 2), 1);			
			}
		}
	}
	else if (errno == EACCES)
		return (ft_putstr_fd("Permission denied\n", 2), 126);
	else if (errno == ENAMETOOLONG)
		return (ft_putstr_fd("Filename too long\n", 2), 126);
	else if (errno == ENOENT)
		return (ft_putstr_fd("No such file or directory\n", 2), 127);
	else
	{
		shell->exit_shell = 1;
		return (ft_putstr_fd("Error calling access\n", 2), 1);
	}
	return (0);
}
