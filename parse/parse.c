/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:06:07 by mateo             #+#    #+#             */
/*   Updated: 2024/06/11 20:33:17 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/* NOTE: PARSING CODE DOESN'T CATER FOR HEREDOCS*/

/*	parse_tokens
	- parses tokens linked list into "ast" linked list
	- parses ast linked list into ast
	- returns 1 if malloc error creating ast nodes or strdup*/
t_ast	*parse_tokens(t_token **tokens)
{
	t_ast	*ast_list;
	t_ast	*root;

	ast_list = ast_list_new(tokens);
	if (!ast_list)
		return (NULL);
	ast_list_print(ast_list);
	root = ast_tree_new(&ast_list);
	return (root);
}
