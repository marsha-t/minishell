/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:58:24 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 17:19:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_logical_op(int code)
{
	if (code == TOKEN_OR || code == TOKEN_AND)
		return (1);
	return (0);
}

void	ast_tree_setup(t_ast **node, t_ast **root)
{
	if ((*node)->code == TOKEN_CMD)
	{
		if ((*node)->next && (*node)->next->code == TOKEN_PIPE)
			*root = ast_pipeline_new(node);
		else
		{
			*root = *node;
			*node = (*node)->next;
		}
	}
	else if ((*node)->code == TOKEN_OBRACKET)
	{
		*node = (*node)->next;
		*root = ast_tree_new(node);
		if ((*node) && (*node)->code == TOKEN_PIPE)
			(*root)->pipe = ast_pipeline_new(node);
	}
}

void	ast_tree_new_cmd(t_ast **node, t_ast **root)
{
	if ((*node)->next && (*node)->next->code == TOKEN_PIPE)
		(*root)->right = ast_pipeline_new(node);
	else
	{
		(*root)->right = *node;
		*node = (*node)->next;
	}
}

void	ast_tree_new_obracket(t_ast **node, t_ast **root)
{
	*node = (*node)->next;
	(*root)->right = ast_tree_new(node);
	if ((*node) && (*node)->code == TOKEN_PIPE)
		(*root)->right->pipe = ast_pipeline_new(node);
}

/*	ast_tree_new generates binary tree for command
	- nodes are commands, && or ||
	- pipelines are linked lists 
	- ast_tree_setup starts the root for the tree */
t_ast	*ast_tree_new(t_ast **node)
{
	t_ast	*root;

	ast_tree_setup(node, &root);
	while (*node)
	{
		if ((*node)->code == TOKEN_CMD)
			ast_tree_new_cmd(node, &root);
		else if ((*node)->code == TOKEN_OBRACKET)
			ast_tree_new_obracket(node, &root);
		else if (is_logical_op((*node)->code) == 1)
		{
			(*node)->left = root;
			root = *node;
			*node = (*node)->next;
		}
		else if ((*node)->code == TOKEN_CBRACKET)
		{
			*node = (*node)->next;
			return (root);
		}
	}
	return (root);
}
