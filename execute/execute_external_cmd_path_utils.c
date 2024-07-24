/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd_path_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:41:51 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 12:41:59 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_array(char **arr, int i)
{
	while (i--)
		free(arr[i]);
	free(arr);
}

/*	check_filepath checks whether a filepath can be accessed
	- returns exit_status depending on error trying to access file */
int	check_filepath(char *cmd, t_shell *shell)
{
	int			error;
	struct stat	file_stat;

	error = access(cmd, X_OK);
	if (error == 0)
	{
		if (stat(cmd, &file_stat) == -1)
			return (shell->exit_shell = 1, err_printf(" stat\n"), 1);
		else if (S_ISDIR(file_stat.st_mode))
			return (err_printf("%s: Is a directory\n", cmd), 126);
		else
			return (0);
	}
	else if (errno == EACCES)
		return (err_printf("%s: Permission denied\n", cmd), 126);
	else if (errno == ENAMETOOLONG)
		return (err_printf("%s: File name too long\n", cmd), 126);
	else if (errno == ENOENT)
		return (err_printf("%s: No such file or directory\n", cmd), 127);
	else if (errno == ENOTDIR)
		return (err_printf("%s: Not a directory\n", cmd), 126);
	else
		return (shell->exit_shell = 1, err_printf("error calling access\n"), 1);
}

/*	has_current_wd returns the position of the
colon representing current working directory
	- if current working directory isn't listed, return -1 */
int	has_current_wd(char *path)
{
	int	i;

	if (path[0] == ':')
		return (0);
	else if (path[ft_strlen(path) - 1] == ':')
		return (ft_strlen(path) - 1);
	else
	{
		i = 0;
		while (path[i])
		{
			if (path[i] == ':' && path[i + 1] == ':')
				return (i);
			i++;
		}
		return (-1);
	}
}

/*	add_current_wd adds the current working directory to path
	and does so in the correct order */
char	*add_current_wd(char *path, int i, t_shell *shell)
{
	char	*dir;
	char	*new_path;

	dir = ft_getcwd(shell);
	if (!dir)
		return (free(path), NULL);
	if (i == 0)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(dir, ft_strdup(":")), path);
		if (!new_path)
			return (err_printf("malloc error: new_path\n"), NULL);
	}
	else if (i == (int)ft_strlen(path) - 1)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(path, ":"), dir);
		if (!new_path)
			return (err_printf("malloc error: new_path\n"), NULL);
	}
	else
		new_path = create_new_path(i, path, dir);
	free_num(2, dir, path);
	return (new_path);
}
