/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:38:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/14 18:36:22 by mateo            ###   ########.fr       */
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

	if (stat(directory, &file_stat)	== -1)
		return (err_printf("minishell: %s: error calling stat\n"), -1);
	if (!S_ISDIR(file_stat.st_mode))
		return (1);
	else
		return (0);
}

/*	match_directory used for cmds with ./ or ../
	- used recursively to handle arbitrary number of directories
	- returns 1 if malloc error or stat fails (in check_directory)
	- returns 0 if no such error (even if no matches)*/
int	match_directory(char *cmd, char *slash, t_dconts **matched, char *directory)
{
	char	*pattern;
	char	*next_slash;
	t_dconts	*dir_contents;
	char		*new_directory;
	
	while (*slash == '/')
		slash++;
	if (*slash == '\0')
	{
		if (check_directory(directory) == 0)
		{
			new_directory = ft_strjoin_free(ft_strdup(directory), ft_strdup("/"));
			if (!new_directory)
				return (free(directory), 1);
			if (create_conts_node(new_directory, matched) == 1)
				return (free_num(2, directory, new_directory), 1);
			else
				return (free(directory), 0);
		}
		else if (check_directory(directory) == -1)
			return (free(directory), 1);
		else
			return (free(directory), 0);
	}
	next_slash = ft_strchr(slash, '/');
	if (next_slash)
		pattern = strdup_range(slash, next_slash - 1);
	else
		pattern = strdup_range(slash, &cmd[ft_strlen(cmd) - 1]);
	if (!pattern)
		return (free(directory), err_printf("minishell: malloc error: strdup_range\n"), 1);
	if (check_directory(directory) == 0)
	{
		dir_contents = create_conts_list(directory);
		if (!dir_contents)
			return (free_num(2, directory, pattern), 1);
	}
	else if (check_directory(directory) == -1)
		return (free_num(2, directory, pattern), 1);
	else
		return (free_num(2, directory, pattern), 0);
	while (dir_contents)
	{
		if (match_pattern_str(pattern, dir_contents->cont_name) == 0)
		{
			new_directory = strjoin_num_free(3, ft_strdup(directory), ft_strdup("/"), ft_strdup(dir_contents->cont_name));
			if (!new_directory)
				return (free_num(2, directory, pattern), free_conts_list(dir_contents), 1);
			if (next_slash)
			{
				if (match_directory(cmd, next_slash, matched, ft_strdup(new_directory)) == 1)
					return (free_num(3, directory, pattern, new_directory), free_conts_list(dir_contents), 1);
			}
			else
			{
				if (create_conts_node(new_directory, matched) == 1)
					return (free_num(3, directory, pattern, new_directory), free_conts_list(dir_contents), 1);
			}
			free(new_directory);
		}
		dir_contents = dir_contents->next;
	}
	free_num(2, directory, pattern);
	free_conts_list(dir_contents);
	return (0);
}

/*	count_matches counts number of nodes 
	in t_dconts linked list */
int	count_matches(t_dconts *matched)
{
	int	count;
	
	count = 0;
	while (matched)
	{
		count++;
		matched = matched->next;
	}
	return (count);
}

/*	convert_matched_to_arg converts matched list of directory contents 
	into linked list of arguments
	- returns number of new arguments
	- returns -1 if malloc error 
	- frees matched list  */
int	convert_matched_to_arg(t_dconts *matched, t_list **new_args, t_list **end_args)
{
	t_dconts	*next_match;
	t_list		*new;
	int			count;

	count = 0;
	while (matched)
	{
		next_match = matched->next;
		new = malloc(sizeof(t_list));
		if (!new)
		{
			free_conts_list(matched);
			return (err_printf("minishell: malloc error: t_list for convert_matched_to_arg\n"), -1);
		}
		new->content = matched->cont_name;
		new->next = NULL;
		if (!*new_args)
			*new_args = new;
		else
			(*end_args)->next = new;
		*end_args = new;
		free(matched);
		matched = next_match;
		count++;
	}
	return (count);
}

/*	add_matched_to_arg inserts list of matched directory contents into args
	- updates node->n_args 
	- returns 1 if convert_matched_to_arg has malloc error */
int add_matched_to_arg(t_dconts *matched, t_list **arg, t_ast *node)
{
	t_list	*new_args;
	t_list	*end_args;
	int		num_args;
	
	new_args = NULL;
	end_args = NULL;
	num_args = convert_matched_to_arg(matched, &new_args, &end_args);
	if (num_args == -1)
		return (1);
	if (!*arg)
		*arg = new_args;
	else
	{
		end_args->next = (*arg)->next;
		(*arg)->next = new_args;
	}
	node->n_args += num_args;
	return (0);
}

