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
	t_ast	*dup_list;
	t_token	*dup_token;

	dup_token = shell->tokens;
	shell->ast_list = ast_list_new(&dup_token);
	if (!shell->ast_list)
		return (1);
	dup_list = shell->ast_list;
	shell->root = ast_tree_new(&dup_list);
	if (!shell->root)
		return (1);
	printf("\n\n printing tree\n");
	ast_tree_print(shell->root);
	return (0);
}
