/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 06:26:27 by mateo             #+#    #+#             */
/*   Updated: 2024/07/07 16:38:59 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	free_num frees an arbitrary number of pointers
	num = number of pointers to be freed*/
void	free_num(int num, ...)
{
	va_list	args;

	va_start(args, num);
	while (num--)
	{
		free(va_arg(args, void *));
	}
	va_end(args);
}

/*	check_filepath checks whether a filepath can be accessed
	- returns exit_status depending on error trying to access file */
// work in progress: catered for other errno as error calling access - this should terminate shell?
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
			return (err_printf("minishell: error calling stat\n"), 1);
		}
		else if (S_ISDIR(file_stat.st_mode))
			return (ft_putstr_fd("minishell: Is a directory\n", 2), 126);
		else
			return (0);
	}
	else if (errno == EACCES)
		return (ft_putstr_fd("minishell: Permission denied\n", 2), 126);
	else if (errno == ENAMETOOLONG)
		return (ft_putstr_fd("minishell: File name too long\n", 2), 126);
	else if (errno == ENOENT)
		return (ft_putstr_fd("minishell: No such file or directory\n", 2), 127);
	else
	{
		shell->exit_shell = 1;
		return (err_printf("minishell: error calling access\n"), 1);
	}
}


/*	free_char_dp frees a char double pointer that is null-terminated*/
void	free_char_dp(char **dp)
{
	int	i;

	i = 0;
	while (dp[i])
	{
		free(dp[i]);
		i++;
	}
	free(dp);
}

/*	has_current_wd returns the position of the colon representing current working directory
	- if current working directory isn't listed, return -1 */
int	has_current_wd(char *path)
{
	int i;

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
	// int	size;
	char *new_path;
	int	j;
	int	k;

  dir = ft_getcwd(shell);
	if (!dir)
	{
		shell->exit_shell = 1;	
		return (free(path), NULL);
	}
	if (i == 0)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(dir, ft_strdup(":")), path);
		if (!new_path)
		{
			shell->exit_shell = 1;
			return (err_printf("minishell: malloc creating new_path\n"), NULL);
		}
	}
	else if (i == (int)ft_strlen(path) - 1)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(path, ":"), dir);
		if (!new_path)
		{
			shell->exit_shell = 1;
			return(err_printf("minishell: malloc: new_path\n"), NULL);
		}
	}
	else
	{
		new_path = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(dir) + 2));
		if (!new_path)
		{
			free_num(2, dir, path);
			shell->exit_shell = 1;
			return (err_printf("minishell: malloc error: new_path\n"), NULL);
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
	- PATH may not exist if it has been unset or set to null
		- return null, exit_status = 127 */
// work in progress: what about other errors from access
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell)
{
	char	*value;
	char	**paths;
	char	*path_cmd;
	struct stat file_stat;
	int	denied;
	int	paths_index;

	value = expand_var("PATH", shell->var_list);
	if (!value)
	{
		*exit_status = 1;
		shell->exit_shell = 1;
		return (ft_putstr_fd("minishell: No such file or directory\n", 2), NULL);
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
		return (err_printf("minishell: malloc error splitting PATH\n"), NULL);
	}
	paths_index = 0;
	while (paths[paths_index])
	{
		path_cmd = ft_strjoin_free(ft_strjoin_free(ft_strdup(paths[paths_index]), ft_strdup("/")), ft_strdup(cmd));
		if (!path_cmd)
		{
			free_char_dp(paths);
			shell->exit_shell = 1;
			return (err_printf("minishell: malloc error: path_cmd\n"), NULL);
		}
		if (access(path_cmd, X_OK) == 0)
		{
			if (stat(path_cmd, &file_stat) == -1)
			{
				*exit_status = 1;
				shell->exit_shell = 1;
				free(path_cmd);
				free_char_dp(paths);
				return (err_printf("minishell: error calling stat\n"), NULL);
			}
			else if (!S_ISDIR(file_stat.st_mode))
			{
				free_char_dp(paths);
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
		return (ft_putstr_fd("minishell: Permission denied\n", 2), NULL);
	}
	*exit_status = 127;
	return (ft_putstr_fd("minishell: Command not found\n", 2), NULL);
}
