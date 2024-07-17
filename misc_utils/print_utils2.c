/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:13:11 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 16:13:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ast_pipeline_print prints the pipes linked list*/
void	ast_pipeline_print(t_ast *node)
{
	int	i;

	i = 1;
	while (node)
	{
		ft_printf("\tpipe %d: %s\n", i, node->cmd);
		i++;
		node = node->pipe;
	}
}
/*	ast_tree_print prints all the nodes in a tree
	- prints node, left, right
	- before going to left and doing the same
	- and then to right and doing the same*/
void	ast_tree_print(t_ast *node)
{
	ft_printf("node: %d, %s\n", node->code, node->cmd);
	if (node->left)
		ft_printf("\tleft: %d, %s\n", node->left->code, node->left->cmd);
	if (node->right)
		ft_printf("\tright: %d, %s\n", node->right->code, node->right->cmd);
	if (node->pipe)
		ast_pipeline_print(node);
	ft_printf("\n\n");
	if (node->left)
		ast_tree_print(node->left);
	if (node->right)
		ast_tree_print(node->right);
}

/*	print_conts_list prints files in list of directory contents*/
void	print_conts_list(t_dconts *list)
{
	t_dconts	*current;

	current = list;
	while (current)
	{
		ft_printf("file: %s\n", current->cont_name);
		current = current->next;
	}
}
