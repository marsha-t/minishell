/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 06:26:27 by mateo             #+#    #+#             */
/*   Updated: 2024/06/19 13:18:58 by mateo            ###   ########.fr       */
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
// work in progress: to terminate shell when error calling stat
// work in progress: catered for other errno as error calling access - this should terminate shell?
int	check_filepath(char *cmd)
{
	int	error;
	struct stat file_stat;
	
	error = access(cmd, X_OK);
	if (error == 0)
	{
		if (stat(cmd, &file_stat) == -1)
			return (ft_putstr_fd("Error calling stat\n", 2), 1); // note: this should terminate shell
		else if (S_ISDIR(file_stat.st_mode))
			return (ft_putstr_fd("Is a directory\n", 2), 126);
		else
			return (0);
	}
	else if (errno == EACCES)
		return (ft_putstr_fd("Permission denied\n", 2), 126);
	else if (errno == ENAMETOOLONG)
		return (ft_putstr_fd("Filename too long\n", 2), 126);
	else if (errno == ENOENT)
		return (ft_putstr_fd("No such file or directory\n", 2), 127);
	else
		return (ft_putstr_fd("Error calling access\n", 2), 1); // note: this should terminate shell
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
		return (free(path), NULL); // terminate shell
	if (i == 0)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(dir, ft_strdup(":")), path);
		if (!new_path)
			return (ft_putstr_fd("Malloc creating new_path\n", 2), NULL); // terminate shell
	}
	else if (i == (int)ft_strlen(path) - 1)
	{
		new_path = ft_strjoin_free(ft_strjoin_free(path, ":"), dir);
		if (!new_path)
			return(ft_putstr_fd("Malloc creating new_path\n", 2), NULL); // terminate shell
	}
	else
	{
		new_path = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(dir) + 2));
		if (!new_path)
		{
			free_num(2, dir, path);
			return (ft_putstr_fd("Malloc error creating new_path\n", 2), NULL); // terminate shell
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
// work in progress: depends on deconflict of value
// work in progress: what about other errors from access 
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell)
{
	char	*value;
	char	**paths;
	char	*path_cmd;
	struct stat file_stat;
	int	denied;
	
	value = expand_var("PATH", shell->var_list);
	if (!value)
	{
		*exit_status = 1;
		return (ft_putstr_fd("No such file or directory\n", 2), NULL);
	}
	if (has_current_wd(value) > -1)
	{
		value = add_current_wd(value, has_current_wd(value), shell);
		if (!value) // malloc error 
		{
			*exit_status = 1;
			return (NULL); // need to terminate shell
		}
	}
	paths = ft_split(value, ':');
	free(value);
	if (!paths)
	{
		*exit_status = 1;
		return (ft_putstr_fd("Malloc error splitting PATH\n", 2), NULL);
	}
	while (*paths)
	{
		path_cmd = ft_strjoin_free(ft_strjoin_free(ft_strdup(*paths), ft_strdup("/")), ft_strdup(cmd));
		if (!path_cmd)
		{
			free_char_dp(paths);
			return (ft_putstr_fd("Malloc error creating path_cmd\n", 2), NULL); // terminate shell
		}
		if (access(path_cmd, X_OK) == 0)
		{
			if (stat(path_cmd, &file_stat) == -1)
			{
				*exit_status = 1;
				free(path_cmd);
				free_char_dp(paths);
				return (ft_putstr_fd("Error calling stat\n", 2), NULL); // need to terminate shell 
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
		paths++;
	}
	free_char_dp(paths);
	if (denied == 1)
	{
		*exit_status = 126;
		return (ft_putstr_fd("Permission denied\n", 2), NULL);
	}
	*exit_status = 127;
	return (ft_putstr_fd("Command not found\n", 2), NULL);
}
