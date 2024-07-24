/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_match_dir2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:51:46 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:52:07 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		{
			rm_dot_dconts(dcont);
			return (0);
		}
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

int	match_dir_while2(char *dir, t_dconts *curr, t_wc *wc_info, char *next_slash)
{
	char	*new_directory;

	new_directory = strjoin_num_free(3, ft_strdup(dir), \
		ft_strdup("/"), ft_strdup(curr->cont_name));
	if (!new_directory)
		return (1);
	if (match_dir_matched(wc_info, next_slash, new_directory) == 1)
		return (1);
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
			if (match_dir_while2(dir, curr, wc_info, next_slash) == 1)
				return (free_num(2, dir, pattern), free_conts_list(dcont), 1);
		}
		curr = curr->next;
	}
	free_num(2, dir, pattern);
	free_conts_list(dcont);
	return (0);
}
