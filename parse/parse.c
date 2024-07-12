/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:06:07 by mateo             #+#    #+#             */
/*   Updated: 2024/06/17 18:41:42 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	parse_tokens
	- parses tokens linked list into "ast" linked list
	- parses ast linked list into ast
	- returns 1 if malloc error creating ast nodes or strdup*/
int	parse_tokens(t_shell *shell)
{
	shell->ast_list = ast_list_new(&shell->tokens);
	if (!shell->ast_list)
		return (1);
	t_ast *dup_list;
	dup_list = shell->ast_list;
	// testing code 
	// ast_list_print(shell->ast_list);
	// t_ast *last;
	// last = shell->ast_list;
	// while (last->next)
	// {
	// 	last = last->next;
	// }
	// printf("last: %d\n", last->code);
	// if (last->next == NULL)
	// 	printf("last->next = NULL\n");

	// shell->root = ast_tree_new(&shell->ast_list);
	shell->root = ast_tree_new(&dup_list);
	printf("\n\n printing tree\n");
	ast_tree_print(shell->root);

	// if (!shell->root)
	// 	return (1);
	return (0);
}
