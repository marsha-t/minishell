/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 06:26:27 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 06:07:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_filepath checks whether a filepath can be accessed
	- returns exit_status depending on error trying to access file */
int	check_filepath(char *cmd, t_shell *shell)
{
	int	error;
	struct stat file_stat;

	error = access(cmd, X_OK);
	if (error == 0)
	{
		if (stat(cmd, &file_stat) == -1)
		{
			shell->exit_shell = 1;
			return (err_printf("error calling stat\n"), 1);
		}
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
	else if (errno = ENOTDIR)
		return (err_printf("%s: Not a directory\n", cmd), 126);
	else
	{
		shell->exit_shell = 1;
		return (err_printf("error calling access\n"), 1);
	}
}


/*	has_current_wd returns the position of the colon representing current working directory
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
char *add_current_wd(char *path, int i, t_shell *shell)
{
	char *dir;
	char *new_path;
	int	j;
	int	k;

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
			return(err_printf("malloc error: new_path\n"), NULL);
	}
	else
	{
		new_path = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(dir) + 2));
		if (!new_path)
		{
			free_num(2, dir, path);
			return (err_printf("malloc error: new_path\n"), NULL);
		}
		j = 0;
		while (j <= i)
		{
			new_path[j] = path[j];
			j++;
		}
		k = 0;
		while (dir[k])
			new_path[j++] = dir[k++];
		i++;
		while (path[i])
			new_path[j++] = path[i++];
	}
	free_num(2, dir, path);
	return (new_path);
}

/*	find_cmd finds the command across the PATH directories
	and checks whether it exists and permissions are granted
	- it also updates exit_status
	- PATH may not exist if it has been unset or set to empty
		- return error: exit_status = 127 */
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell)
{
	char	*value;
	char	**paths;
	char	*path_cmd;
	struct stat file_stat;
	int	denied;
	int	paths_index;

	value = expand_var("PATH", shell->var_list);
	if (value[0] == '\0')
	{
		*exit_status = 127;
		return (err_printf("%s: No such file or directory\n", cmd), NULL);
	}
	else if (!value)
	{
		*exit_status = 1;
		shell->exit_shell = 1;
		return (err_printf("malloc error: expand_var in find_cmd\n"), NULL);
	}
	if (has_current_wd(value) > -1)
	{
		value = add_current_wd(value, has_current_wd(value), shell);
		if (!value)
		{
			*exit_status = 1;
			shell->exit_shell = 1;
			return (NULL);
		}
	}
	paths = ft_split(value, ':');
	free(value);
	if (!paths)
	{
		*exit_status = 1;
		shell->exit_shell = 1;
		return (err_printf("malloc error: ft_split in find_cmd\n"), NULL);
	}
	paths_index = 0;
	while (paths[paths_index])
	{
		path_cmd = ft_strjoin_free(ft_strjoin_free(ft_strdup(paths[paths_index]), ft_strdup("/")), ft_strdup(cmd));
		if (!path_cmd)
		{
			free_char_dp(paths);
			*exit_status = 1;
			shell->exit_shell = 1;
			return (err_printf("malloc error: path_cmd\n"), NULL);
		}
		if (access(path_cmd, X_OK) == 0)
		{
			if (stat(path_cmd, &file_stat) == -1)
			{
				*exit_status = 1;
				shell->exit_shell = 1;
				free(path_cmd);
				free_char_dp(paths);
				return (err_printf("error calling stat\n"), NULL);
			}
			else if (!S_ISDIR(file_stat.st_mode))
			{
				free_char_dp(paths);
				*exit_status = 0;
				return (path_cmd);
			}
		}
		else if (errno == EACCES)
			denied = 1;
		free(path_cmd);
		paths_index++;
	}
	free_char_dp(paths);
	if (denied == 1)
	{
		*exit_status = 126;
		return (err_printf("%s: Permission denied\n", cmd), NULL);
	}
	*exit_status = 127;
	return (err_printf("%s: command not found\n", cmd), NULL);
}
