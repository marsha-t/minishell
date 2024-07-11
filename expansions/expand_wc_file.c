/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:38:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/11 05:39:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	expand_wildcard_file */
int	expand_wildcard_file(t_dconts *list, t_ast *node, char *pattern, int code)
{
	t_dconts *matched_list;
	int	match_count;
	t_file *curr_file;
	t_dconts *next_match;
	// char *temp;

	match_count = expand_wildcard_setup(&matched_list, pattern, list);
	if (match_count == -1)
		return (1); // terminate shell 
	else if (match_count == 0)
		return (free_conts_list(matched_list), 0);
	else if (match_count > 1)
	{
		free_conts_list(matched_list);
		return (err_printf("minishell: %s: ambiguous redirect\n", pattern), 1); // don't terminate shell
	}
	if (code == TOKEN_INPUT)
		curr_file = node->input_list;
	else if (code == TOKEN_OUTPUT)
		curr_file = node->output_list;
	else if (code == TOKEN_HEREDOC)
		curr_file = node->heredoc_list;
	while (curr_file)
	{
		// temp = remove_quote_str(ft_strdup(curr_file->file_name));
		// if (!temp)
		// 	return (1);
		// if (ft_strcmp(temp, pattern) == 0)
		if (ft_strcmp(curr_file->file_name, pattern) == 0)
		{
			// free(temp);
			break;
		}
		curr_file = curr_file->next;
	}
	free(curr_file->file_name);
	curr_file->file_name = matched_list->cont_name;
	next_match = matched_list->next;
	free(matched_list);
	matched_list = next_match;
	if (matched_list)
		free_conts_list(matched_list);
	return (0);
}

int	ast_node_add_file(t_file **file, char *str)
{
	t_file *new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (err_printf("minishell: malloc error: t_file for ast_node_push_file\n"), 1);
	new->file_name = str;
	if (!(*file))
		*file = new;
	else
	{
		new->next = *file;
		*file = new;
	}
	return (0);
}