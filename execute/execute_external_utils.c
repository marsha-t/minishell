/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 06:26:27 by mateo             #+#    #+#             */
/*   Updated: 2024/07/24 16:58:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_new_path(int i, char *path, char	*dir)
{
	int		j;
	char	*new_path;
	int		k;

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
	return (new_path);
}

char	*check_path_validity(char **paths,
char *path_cmd, int *exit_status, t_shell *shell)
{
	struct stat	file_stat;

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
			return (free_char_dp(paths), *exit_status = 0, path_cmd);
	}
	free_char_dp(paths);
	free(path_cmd);
	return (*exit_status = 1, NULL);
}

char	*get_direc(t_shell *shell, int *exit_status, char *cmd)
{
	char	*value;

	value = expand_var("PATH", shell->var_list);
	if (value[0] == '\0')
		return (*exit_status = 127, free(value),
			err_printf("%s: No such file or directory\n", cmd), NULL);
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
			return (*exit_status = 1, shell->exit_shell = 1, NULL);
	}
	return (value);
}

char	**get_pathes(t_shell *shell, int *exit_status, char *cmd)
{
	char	**paths;
	char	*value;

	value = get_direc(shell, exit_status, cmd);
	if (!value)
		return (NULL);
	paths = ft_split(value, ':');
	free(value);
	if (!paths)
		return (*exit_status = 1, shell->exit_shell = 1,
			err_printf("malloc error: ft_split in find_cmd\n"), NULL);
	return (paths);
}

char	*find_cmd_return(char **paths, int denied, int *exit_status, char *cmd)
{
	free_char_dp(paths);
	if (denied == 1)
		return (*exit_status = 126,
			err_printf("%s: Permission denied\n", cmd), NULL);
	return (*exit_status = 127,
		err_printf("%s: command not found\n", cmd), NULL);
}

/*	find_cmd finds the command across the PATH directories
	and checks whether it exists and permissions are granted
	- it also updates exit_status
	- PATH may not exist if it has been unset or set to empty
		- return error: exit_status = 127 */
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell)
{
	char	**paths;
	char	*path_cmd;
	int		denied;
	int		paths_index;

	paths = get_pathes(shell, exit_status, cmd);
	if (!paths)
		return (NULL);
	paths_index = -1;
	denied = 0;
	while (paths[++paths_index])
	{
		path_cmd = ft_strjoin_free(ft_strjoin_free(ft_strdup(paths[paths_index]
						), ft_strdup("/")), ft_strdup(cmd));
		if (!path_cmd || access(path_cmd, X_OK) == 0)
			return (check_path_validity(paths, path_cmd, exit_status, shell));
		else if (errno == EACCES)
		{
			denied = 1;
			free(path_cmd);
		}
		else
			free(path_cmd);
	}
	return (find_cmd_return(paths, denied, exit_status, cmd));
}
