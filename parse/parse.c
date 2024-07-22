/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:06:07 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 16:40:44 by mateo            ###   ########.fr       */
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
	t_token	*dup_tokens;

	dup_tokens = shell->tokens;
	shell->ast_list = ast_list_new(&shell->tokens);
	free_tokens_null(&dup_tokens);
	if (!shell->ast_list)
		return (1);
	dup_list = shell->ast_list;
	shell->root = ast_tree_new(&dup_list);
	if (!shell->root)
		return (1);
	return (0);
}
