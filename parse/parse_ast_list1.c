/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_list1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:02:50 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 18:19:02 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	classify_redir_node(t_ast *node)
{
	while (node)
	{
		if (node->code == 0)
		{
			node->code = TOKEN_CMD;
			node->cmd = ft_strdup("");
			if (!node->cmd)
				return (err_printf("malloc error: ft_strdup\n"), 1);
		}
		node = node->next;
	}
	return (0);
}

int	ast_list_new_while(t_token **tokens, t_ast **current, t_ast **start)
{
	if ((*tokens)->code == TOKEN_CMD && *current && !(*current)->cmd)
	{
		if (ast_node_append_cmd(tokens, *current) == 1)
			return (ast_list_free(*start), 1);
	}
	else if (is_cmdorder_op((*tokens)->code) > 0 || \
		((*tokens)->code == TOKEN_CMD) || \
		((*tokens)->code == TOKEN_CBRACKET))
	{
		if (ast_node_add(tokens, start, current) == 1)
			return (ast_list_free(*start), 1);
	}
	else if ((*tokens)->code == TOKEN_ARG)
	{
		if (ast_node_append_arg(tokens, *current) == 1)
			return (ast_list_free(*start), 1);
	}
	else
	{
		if (ast_node_append_misc(tokens, start, current) == 1)
			return (ast_list_free(*start), 1);
	}
	return (0);
}

/*	ast_list_new generates nodes in the ast
	- ast nodes are placed in a linked list
	- frees linked list
*/
t_ast	*ast_list_new(t_token **tokens)
{
	t_ast	*start;
	t_ast	*current;

	start = 0;
	current = 0;
	while (*tokens)
	{
		if (ast_list_new_while(tokens, &current, &start) == 1)
			return (NULL);
	}
	if (classify_redir_node(start) == 1)
		return (ast_list_free(start), NULL);
	return (start);
}
