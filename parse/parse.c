/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:06:07 by mateo             #+#    #+#             */
/*   Updated: 2024/06/10 21:22:29 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/* NOTE: PARSING CODE DOESN'T CATER FOR HEREDOCS*/

/*	parse_tokens
	- parses tokens linked list into "ast" linked list
	- parses ast linked list into ast
	- returns 1 if malloc error creating ast nodes or strdup*/
t_ast	*parse_tokens(t_token **tokens, t_shell *shell)
{
	t_ast	*ast_list;
	t_ast *curr;
	t_ast	*root;
	int i;
	// char **temp;
	i = 0;

	ast_list = ast_list_new(tokens);
	// t_dconts *list;
	// temp = NULL;
	// int j;
	// j = 0;
	curr = ast_list;
	while (curr)
	{
		if(curr->args)
		{	while (curr->args)
			{
				printf("%s",curr->args->content);
				if(contain_var(curr->args->content)== 0)
					curr->args->content = expand_str(curr->args->content,shell->var_list);
				curr->args = curr->args->next;
			}
		}
		curr = curr->next;
	}
	if (!ast_list)
		return (NULL);
	ast_list_print(ast_list);
	root = ast_tree_new(&ast_list);
	return (root);
}
