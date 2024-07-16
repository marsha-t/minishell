/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:06:07 by mateo             #+#    #+#             */
/*   Updated: 2024/07/16 07:47:02 by mateo            ###   ########.fr       */
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
	
	shell->ast_list = ast_list_new(&shell->tokens);
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
