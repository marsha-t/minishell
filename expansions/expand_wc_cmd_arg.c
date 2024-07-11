/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_cmd_arg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/07/11 05:43:00 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int expand_wildcard_setup(t_dconts **matched_list, char *pattern, t_dconts *list)
{
	int			match_count;
	char 		*unquoted;

	unquoted = remove_quote_str(ft_strdup(pattern));
	if (!unquoted)
		return (-1); // terminate shell; no need err msg
	match_count = match_pattern_list(unquoted, list, matched_list);
	free(unquoted);
	return (match_count);
}

/*	expand_wildcard_cmd 
	- removes quotes in cmd 
	- merges strings in matched linked list into node: 
		- first match replaces cmd
		- other matches added to arg in reverse order
	- returns 1 if malloc error, 0 otherwise (matches or no matches both return 0)
		*/
// work in progress: check free with malloc error
int	expand_wildcard_cmd(t_dconts *list, t_ast *node)
{
	t_dconts	*matched_list;
	t_dconts	*next;
	int	match_count; 

	match_count = expand_wildcard_setup(&matched_list, node->cmd, list);
	if (match_count == -1)
		return (free_conts_list(matched_list), 1);
	else if (match_count == 0)
		return (free_conts_list(matched_list), 0);
	next = matched_list->next;
	free(node->cmd);
	node->cmd = matched_list->cont_name;
	free(matched_list);
	matched_list = next;
	while (matched_list)
	{
		next = matched_list->next;
		if (ast_node_add_arg(&node->args, matched_list->cont_name) == 1)
		{
			free_conts_list(matched_list);
			return (err_printf("minishell: malloc error: ast_node_push_arg\n"), 1);
		}
		node->n_args++;
		free(matched_list);
		matched_list = next;
	}
	return (0);
}

/*	expand_wildcard_arg
	- removes quotes in specific arg (i.e., pattern)
	- adds expanded matches into node args in reverse order
	- returns 1 if malloc error, 0 otherwise (matches or no matches both return 0)
	 */
// work in progress: check free with malloc error
int	expand_wildcard_arg(t_dconts *list, t_ast *node, char *pattern)
{
	int match_count;
	t_dconts *matched_list;
	t_list *curr_arg;
	t_dconts *next_match;
	
	match_count = expand_wildcard_setup(&matched_list, pattern, list);
	if (match_count == -1)
		return (free_conts_list(matched_list), 1);
	else if (match_count == 0)
		return (free_conts_list(matched_list), 0);
	curr_arg = node->args;
	while (curr_arg)
	{
		if (ft_strcmp(curr_arg->content, pattern) == 0)
		{
			// free(temp);
			break;
		}
		curr_arg = curr_arg->next;
	}
	free(curr_arg->content);
	curr_arg->content = matched_list->cont_name;
	next_match = matched_list->next;
	free(matched_list);
	matched_list = next_match;
	while (matched_list)
	{
		next_match = matched_list->next;
		if (ast_node_add_arg(&curr_arg->next, matched_list->cont_name) == 1)
		{
			free_conts_list(matched_list);
			return (err_printf("minishell: malloc error: ast_node_push_arg\n"), 1);
		}
		node->n_args++;
		free(matched_list);
		matched_list = next_match;
	}
	return (0);
}

/*	ast_node_add_arg adds a new arg to start of list
	- uses pointer to str (i.e., doesn't duplicate it)
	- increment n_args */
int	ast_node_add_arg(t_list **arg, char *str)
{
	t_list *new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (err_printf("minishell: malloc error: t_ast for ast_node_push_arg\n"), 1);
	new->content = str;
	if (!(*arg))
		*arg = new;
	else
	{
		new->next = *arg;
		*arg = new;
	}
	return (0);
}
