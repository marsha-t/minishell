/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:38:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/20 15:03:32 by mateo            ###   ########.fr       */
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

/*	NOTE TO RAGHDA: match_dir is the original function I created
	but it is too long.
	I've split it into more functions
	and included the use of a structure (t_wc) to deal with some norminette issues.
	But after the split, it may be harder to understand the logic so I've left the original function here
	We can remove later when dealing with norminette :)

	The new functions are:
		- match_dir
			- match_dir_end
			- match_dir_while
				- match_dir_contents
				- match_dir_matched

	The nested list above shows which functions calls on which function
	(e.g., match_dir calls on match_dir_end and match_dir_while)
*/

// /*	match_dir used for cmds with ./ or ../
// 	- used recursively to handle arbitrary number of directories
// 	- returns 1 if malloc error or stat fails (in check_directory)
// 	- returns 0 if no such error (even if no matches)*/
// int	match_dir(char *cmd, char *slash, t_dconts **matched, char *directory)
// {
// 	char	*pattern;
// 	char	*next_slash;
// 	t_dconts	*dir_contents;
// 	char		*new_directory;

// 	while (*slash == '/')
// 		slash++;
// 	if (*slash == '\0')
// 	{
// 		if (check_directory(directory) == 0)
// 		{
// 			new_directory = ft_strjoin_free(ft_strdup(directory), ft_strdup("/"));
// 			if (!new_directory)
// 				return (free(directory), 1);
// 			if (create_conts_node(new_directory, matched) == 1)
// 				return (free_num(2, directory, new_directory), 1);
// 			else
// 				return (free(directory), 0);
// 		}
// 		else if (check_directory(directory) == -1)
// 			return (free(directory), 1);
// 		else
// 			return (free(directory), 0);
// 	}
// 	next_slash = ft_strchr(slash, '/');
// 	if (next_slash)
// 		pattern = strdup_range(slash, next_slash - 1);
// 	else
// 		pattern = strdup_range(slash, &cmd[ft_strlen(cmd) - 1]);
// 	if (!pattern)
// 		return (free(directory), err_printf("malloc error: strdup_range\n"), 1);
// 	if (check_directory(directory) == 0)
// 	{
// 		dir_contents = create_conts_list(directory);
// 		if (!dir_contents)
// 			return (free_num(2, directory, pattern), 1);
// 	}
// 	else if (check_directory(directory) == -1)
// 		return (free_num(2, directory, pattern), 1);
// 	else
// 		return (free_num(2, directory, pattern), 0);
// 	while (dir_contents)
// 	{
// 		if (match_pattern_str(pattern, dir_contents->cont_name) == 0)
// 		{
// 			new_directory = strjoin_num_free(3, ft_strdup(directory), ft_strdup("/"), ft_strdup(dir_contents->cont_name));
// 			if (!new_directory)
// 				return (free_num(2, directory, pattern), free_conts_list(dir_contents), 1);
// 			if (next_slash)
// 			{
// 				if (match_dir(cmd, next_slash, matched, ft_strdup(new_directory)) == 1)
// 					return (free_num(3, directory, pattern, new_directory), free_conts_list(dir_contents), 1);
// 			}
// 			else
// 			{
// 				if (create_conts_node(new_directory, matched) == 1)
// 					return (free_num(3, directory, pattern, new_directory), free_conts_list(dir_contents), 1);
// 			}
// 			free(new_directory);
// 		}
// 		dir_contents = dir_contents->next;
// 	}
// 	free_num(2, directory, pattern);
// 	free_conts_list(dir_contents);
// 	return (0);
// }

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

/*	match_dcont sets up dcont
	- checks whether dir is really a directory
		- if so, create dcont
	- returns
		0 if successfully created dcont
		-1 if malloc error or error checking dir using stat
		1 if dir is not a directory
	*/
int	match_dcont(char *dir, char *pattern, t_dconts **dcont)
{
	if (check_directory(dir) == 0)
	{
		*dcont = create_conts_list(dir);
		if (!*dcont)
			return (free_num(2, dir, pattern), -1);
		else
			return (0);
	}
	else if (check_directory(dir) == -1)
		return (free_num(2, dir, pattern), -1);
	else
		return (free_num(2, dir, pattern), 1);
}

/*	match_dir_matched adds matched files into matched linked list
	- calls match_dir recursively if there are more directories to parse
	- if no more directories, add matched file to matched linked list
	- returns 1 if malloc and stat errors */
int	match_dir_matched(t_wc *wc_info, char *next_slash, char *new_directory)
{
	if (next_slash)
	{
		if (match_dir(wc_info, next_slash, ft_strdup(new_directory)) == 1)
			return (free(new_directory), 1);
	}
	else
	{
		if (create_conts_node(new_directory, &wc_info->matched) == 1)
			return (free(new_directory), 1);
	}
	free(new_directory);
	return (0);
}

/*	match_dir_while
	- sets up dcont using match_dcont
	- matches pattern with dcont
	- add matched files into matched linked list
		using match_dir_matched */
int	match_dir_while(char *dir, char *pattern, char *next_slash, t_wc *wc_info)
{
	t_dconts	*dcont;
	char		*new_directory;
	int			dcont_return;
	t_dconts	*curr;

	dcont_return = match_dcont(dir, pattern, &dcont);
	if (dcont_return == -1)
		return (1);
	else if (dcont_return == 1)
		return (0);
	curr = dcont;
	while (curr)
	{
		if (match_pattern_str(pattern, curr->cont_name) == 0)
		{
			new_directory = strjoin_num_free(3, ft_strdup(dir), ft_strdup("/"), ft_strdup(curr->cont_name));
			if (!new_directory)
				return (free_num(2, dir, pattern), free_conts_list(dcont), 1);
			if (match_dir_matched(wc_info, next_slash, new_directory) == 1)
				return (free_num(2, dir, pattern), free_conts_list(dcont), 1);
		}
		curr = curr->next;
	}
	free_num(2, dir, pattern);
	free_conts_list(dcont);
	return (0);
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

/*	create_arg allocates arg node and initialises it using arg_str*/
t_list	*create_arg(char *arg_str)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
	{
		err_printf("malloc error: t_list for matched_to_arg\n");
		return (NULL);
	}
	new->content = arg_str;
	new->next = NULL;
	return (new);
}

/*	convert_matched_to_arg converts matched list of directory contents
	into linked list of arguments
	- returns number of new arguments
	- returns -1 if malloc error
	- frees matched list  */
int	matched_to_arg(t_dconts *matched, t_list **new_args, t_list **end_args)
{
	t_dconts	*next_match;
	t_list		*new;
	int			count;

	count = 0;
	while (matched)
	{
		next_match = matched->next;
		new = create_arg(matched->cont_name);
		if (!new)
			return (free_conts_list(matched), -1);
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
	- returns 1 if matched_to_arg has malloc error */
int	add_matched_to_arg(t_dconts *matched, t_list **arg, t_ast *node)
{
	t_list	*new_args;
	t_list	*end_args;
	int		num_args;

	if (!matched)
		return (0);
	new_args = NULL;
	end_args = NULL;
	num_args = matched_to_arg(matched, &new_args, &end_args);
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

/*	init_wc allocates and initialises wc_info */
int	init_wc(t_wc **wc_info, char *cmd)
{
	*wc_info = malloc(sizeof(t_wc));
	if (!*wc_info)
		return (err_printf("malloc error: wc_info\n"), 1);
	(*wc_info)->matched = NULL;
	(*wc_info)->cmd = cmd;
	return (0);
}

/*	free_wc_info frees matched linked list as well as wc struct */
void	free_wc_info(t_wc *wc_info)
{
	free_conts_list(wc_info->matched);
	if (wc_info->matched)
		printf("not null\n");
	else
		printf("null\n");
	free(wc_info);
}
