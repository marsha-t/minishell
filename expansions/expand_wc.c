/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/07/23 15:57:08 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		match_count = expand_wc_cmd_slash(node, wc_info, slash);
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

int	expand_wc_file_add(int code, t_ast *node, char *pattern, t_dconts *matched)
{
	t_file		*curr_file;

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
	curr_file->file_name = matched->cont_name;
	free(matched);
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

	match_count = expand_wc_setup(&matched_list, pattern, \
		&shell->directory_contents);
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
	expand_wc_file_add(code, node, pattern, matched_list);
	return (0);
}
