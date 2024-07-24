/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:17:51 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 17:19:39 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ast_pipeline_new_cmd(t_ast **node, t_ast **left, t_ast **head)
{
	if (!*left)
		*left = *node;
	else
	{
		(*left)->pipe = *node;
		*left = *node;
	}
	if (!*head)
		*head = *left;
	*node = (*node)->next;
}

void	ast_pipeline_new_obracket(t_ast **node, t_ast **left, t_ast **head)
{
	*node = (*node)->next;
	if (!*left)
		*left = ast_tree_new(node);
	else
	{
		(*left)->pipe = ast_tree_new(node);
		*left = (*left)->pipe;
	}
	if (!*head)
		*head = *left;
}

/*	ast_pipeline_new creates linked list for pipeline
	- if node is cmd: call on ast_pipeline_new_cmd
	- if node is open parenthesis: call on ast_pipeline_new_obracket 
		(to recursively call ast_tree_new) */
t_ast	*ast_pipeline_new(t_ast **node)
{
	t_ast	*head;
	t_ast	*left;

	head = NULL;
	left = NULL;
	while (*node)
	{
		if ((*node)->code == TOKEN_PIPE)
			*node = (*node)->next;
		else if ((*node)->code == TOKEN_CMD)
			ast_pipeline_new_cmd(node, &left, &head);
		else if ((*node)->code == TOKEN_OBRACKET)
			ast_pipeline_new_obracket(node, &left, &head);
		else if ((*node)->code == TOKEN_CBRACKET)
			break ;
		else if (is_logical_op((*node)->code) == 1)
			break ;
	}
	return (head);
}
