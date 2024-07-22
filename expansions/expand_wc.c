/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/07/21 17:00:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_dot_wc(char *pattern)
{
	if (pattern[0] == '.')
		return (0);
	else if (ft_strncmp(pattern, "\".", 2) == 0)
		return (0);
	else if (ft_strncmp(pattern, "\'.", 2) == 0)
		return (0);
	return (1);
}

void	rm_dot_dconts(t_dconts **list)
{
	t_dconts	*curr;
	t_dconts	*temp;
	t_dconts	*prev;
	
	curr = *list;
	prev = NULL;
	while (curr)
	{
		if (curr->cont_name[0] == '.')
		{
			if (!prev)
				*list = curr->next;
			else
				prev->next = curr->next;
			temp = curr;
			curr = curr->next;
			free(temp->cont_name);
			free(temp);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

/*	expand_wc_setup
	- removes . or .. files from dir contents list 
		if pattern starts with dot (quoted or unquoted)
	- generate list of matches
	- returns number of matches
	- returns -1 if error removing quote or generating matches
		- error handling for match_pattern_list done by
			function that calls expand_wc_setup */
int	expand_wc_setup(t_dconts **matched_list, char *pattern, t_dconts **list)
{
	int		match_count;

	if (start_dot_wc(pattern) == 1)
		rm_dot_dconts(list);
	match_count = match_pattern_list(pattern, *list, matched_list);
	return (match_count);
}

/*	expand_wc_cmd
	- generates list of matched contents
	- merges strings in matched linked list into node:
		- first match replaces cmd
		- other matches added to arg in order
	- returns 1 if malloc error,
		0 otherwise (matches or no matches both return 0) */
		// work in progresS: need to free wc_info struct
int	expand_wc_cmd(t_dconts **list, t_ast *node)
{
	t_dconts	*next;
	int			match_count;
	char		*slash;
	t_wc		*wc_info;

	if (init_wc(&wc_info, node->cmd) == 1)
		return (1);
	slash = ft_strchr(node->cmd, '/');
	if (slash)
	{
		if (ft_strncmp(node->cmd, "./", 2) == 0)
		{
			if (match_dir(wc_info, slash, ft_strdup(".")) == 1)
				return (free_wc_info(wc_info), 1);
		}
		else if (ft_strncmp(node->cmd, "../", 3) == 0)
		{
			if (match_dir(wc_info, slash, ft_strdup("..")) == 1)
				return (free_wc_info(wc_info), 1);
		}
		else
			return (0);
		match_count = count_matches(wc_info->matched);
	}
	else
		match_count = expand_wc_setup(&wc_info->matched, node->cmd, list);
	if (match_count == -1)
		return (free_wc_info(wc_info), 1);
	else if (match_count == 0)
		return (free_wc_info(wc_info), 0);
	next = wc_info->matched->next;
	free(node->cmd);
	node->cmd = wc_info->matched->cont_name;
	free(wc_info->matched);
	wc_info->matched = next;
	if (add_matched_to_arg(wc_info->matched, &node->args, node) == 1)
		return (free(wc_info), 1);
	return (free(wc_info), 0);
}

/*	expand_wc_arg
	- generates list of matched contents
	- adds expanded matches into args in order
	- returns 1 if malloc error,
		0 otherwise (matches or no matches both return 0) */
int	expand_wc_arg(t_dconts **list, t_ast *node, char *pattern)
{
	int			match_count;
	t_dconts	*matched_list;
	t_list		*curr_arg;
	t_dconts	*next_match;

	match_count = expand_wc_setup(&matched_list, pattern, list);
	if (match_count == -1)
		return (free_conts_list(matched_list), 1);
	else if (match_count == 0)
		return (free_conts_list(matched_list), 0);
	curr_arg = node->args;
	while (curr_arg)
	{
		if (ft_strcmp(curr_arg->content, pattern) == 0)
			break ;
		curr_arg = curr_arg->next;
	}
	free(curr_arg->content);
	curr_arg->content = matched_list->cont_name;
	next_match = matched_list->next;
	free(matched_list);
	matched_list = next_match;
	if (add_matched_to_arg(matched_list, &curr_arg, node) == 1)
		return (1);
	return (0);
}

/*	expand_wc_file
	- generates list of matched contents
	- replaces file with matched file
	- if more than 1 match, return 1 (don't terminate shell)
	- return 1 if malloc error (terminate shell)
	*/
int	expand_wc_file(t_shell *shell, t_ast *node, char *pattern, int code)
{
	t_dconts	*matched_list;
	int			match_count;
	t_file		*curr_file;

	match_count = expand_wc_setup(&matched_list, pattern, &shell->directory_contents);
	if (match_count == -1)
	{
		shell->exit_shell = 1;
		return (1);
	}
	else if (match_count == 0)
		return (free_conts_list(matched_list), 0);
	else if (match_count > 1)
	{
		free_conts_list(matched_list);
		return (err_printf("%s: ambiguous redirect\n", pattern), 1);
	}
	curr_file = NULL;
	if (code == TOKEN_INPUT)
		curr_file = node->input_list;
	else if (code == TOKEN_OUTPUT)
		curr_file = node->output_list;
	while (curr_file)
	{
		if (ft_strcmp(curr_file->file_name, pattern) == 0)
			break ;
		curr_file = curr_file->next;
	}
	free(curr_file->file_name);
	curr_file->file_name = matched_list->cont_name;
	free(matched_list);
	return (0);
}
