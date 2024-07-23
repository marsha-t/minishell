/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_match_dir1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:50:06 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:57:15 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_directory returns 0 if directory is a directory
	- returns 1 if not a directory
	- return -1 if stat fails
	*/
int	check_directory(char *directory)
{
	struct stat	file_stat;

	if (stat(directory, &file_stat) == -1)
		return (err_printf("%s: error calling stat\n"), -1);
	if (!S_ISDIR(file_stat.st_mode))
		return (1);
	else
		return (0);
}

/*	match_dir_end caters for cases that end with slashes
	e.g., ./exp*nsions/
	- checks that dir is really a directory
		- if so, it is added to matched list (e.g., ./exp*nsions/)
		- otherwise, it is not (e.g., ./global*h/)
	- returns 1 if malloc error or error calling stat (in check_directory)
	- returns 0 otherwise (even if no matches)
*/
int	match_dir_end(char *dir, t_dconts **matched)
{
	char	*new_directory;

	if (check_directory(dir) == 0)
	{
		new_directory = ft_strjoin_free(ft_strdup(dir), ft_strdup("/"));
		if (!new_directory)
			return (free(dir), 1);
		if (create_conts_node(new_directory, matched) == 1)
			return (free_num(2, dir, new_directory), 1);
		else
			return (free(dir), 0);
	}
	else if (check_directory(dir) == -1)
		return (free(dir), 1);
	else
		return (free(dir), 0);
}

/*	match_dir used for cmds with ./ or ../
	- used recursively (with other match_dir_* functions)
		to handle arbitrary number of directories
	- returns 1 if malloc error or stat error
	- returns 0 if no such error (even if no matches)*/
int	match_dir(t_wc *wc_info, char *slash, char *dir)
{
	char		*pattern;
	char		*next_slash;
	int			len;

	while (*slash == '/')
		slash++;
	if (*slash == '\0')
		return (match_dir_end(dir, &wc_info->matched));
	next_slash = ft_strchr(slash, '/');
	len = ft_strlen(wc_info->cmd) - 1;
	if (next_slash)
		pattern = strdup_range(slash, next_slash - 1);
	else
		pattern = strdup_range(slash, &wc_info->cmd[len]);
	if (!pattern)
	{
		free(dir);
		return (err_printf("malloc error: strdup_range\n"), 1);
	}
	if (match_dir_while(dir, pattern, next_slash, wc_info) == 1)
		return (1);
	return (0);
}

int	expand_wc_cmd_slash(t_ast *node, t_wc *wc_info, char *slash)
{
	int	match_count;

	if (ft_strncmp(node->cmd, "./", 2) == 0)
	{
		if (match_dir(wc_info, slash, ft_strdup(".")) == 1)
			return (-1);
	}
	else if (ft_strncmp(node->cmd, "../", 3) == 0)
	{
		if (match_dir(wc_info, slash, ft_strdup("..")) == 1)
			return (-1);
	}
	else
		return (0);
	match_count = count_matches(wc_info->matched);
	return (match_count);
}
