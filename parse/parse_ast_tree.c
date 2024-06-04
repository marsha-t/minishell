/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:14:44 by mateo             #+#    #+#             */
/*   Updated: 2024/05/25 18:44:31 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ast_tree_new generates AST from AST linked list 
	- recursively calls itself for brackets (to create whole subtree) */
t_ast	*ast_tree_new(t_ast **node)
{
	t_ast	*root;

	if ((*node)->code == TOKEN_CMD)
	{
		root = *node;
		*node = (*node)->next;
	}
	else if ((*node)->code == TOKEN_OBRACKET)
	{
		*node = (*node)->next;
		root = ast_tree_new(node);
	}
	while (*node)
	{
		if ((*node)->code == TOKEN_CMD)
		{
			root->right = *node;
			*node = (*node)->next;
		}
		else if ((*node)->code == TOKEN_OBRACKET)
		{
			*node = (*node)->next;
			root->right = ast_tree_new(node);
		}
		else if (is_cmdorder_op((*node)->code) == 1)
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

/*	ast_tree_print prints all the nodes in a tree
	- prints node, left, right
	- before going to left and doing the same
	- and then to right and doing the same*/
void	ast_tree_print(t_ast *node)
{
	printf("node: %d, %s\n", node->code, node->cmd);
	if (node->left)
		printf("left: %d, %s\n", node->left->code, node->left->cmd);
	if (node->right)
		printf("right: %d, %s\n", node->right->code, node->right->cmd);
	printf("\n\n");
	if (node->left)
		ast_tree_print(node->left);
	if (node->right)
		ast_tree_print(node->right);
}
