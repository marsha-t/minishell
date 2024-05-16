/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:03:44 by mateo             #+#    #+#             */
/*   Updated: 2024/05/16 17:58:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*init_ast_node(void)
{
	t_ast	*new;
	
	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->cmd = 0;
	new->n_args = 0;
	new->args = 0;
	new->code = 0;
	new->root = 0;
	new->next = 0;
	new->left = 0;
	new->right = 0;
	return (new);
}

/*	new_ast_node creates a new ast node
	- it moves the tokens pointer along as needed */
t_ast	*new_ast_node(t_tokens **tokens)
{
	t_ast	*new;
	
	new = init_ast_node();
	if (!new)
		return (NULL);
	if ((*tokens)->code == TOKEN_CMD)
	while (is_cmdorder_op((*tokens)->code) != 0)
	{
		if ((*tokens)->code == TOKEN_CMD)
		{
			new->cmd = (*token)->str;
			new->code = (*token)->code;
		}
		if ((*tokens)->code == TOKEN_ARG)
		{
			
		}
	}
	if ((*tokens)->code == TOKEN_PIPE)
	{
		new->cmd = (*token)->str;
		new->code = (*token)->code;
	}
	return (new);
}
/*	add_ast_node: 
	- calls on new_ast_node to generate an ast node
	- attaches new node to end of list 
		- current node is included in function to avoid traversal of ast linked list
	- returns 1 if malloc error 
*/
int	add_ast_node(t_token **tokens, t_ast **start, t_ast **current)
{
	t_ast	*new;

	new = new_ast_node(tokens);
	if (!new)
		return (1);
	if (!(*start))
	{
		*start = new;
		*current = start;
	}
	else
	{
		(*current)->next = new;
		(*current) = new;
	}
	return (0);
}

/*	gen_ast_nodes generates nodes in the ast
	- 2 types of ast nodes: cmd and op nodes
	- ast nodes are placed in a linked list */
void	*gen_ast_list(t_token **tokens)
{
	t_ast	*start;
	t_ast	*current;
	
	start = 0;
	current = 0;
	while (*tokens)
	{		
		add_ast_node(tokens, &start, &current);
		tokens = tokens->next;	
	}
	// free(tokens);
	return (start);
}
// void	rpn_tokens(t_token **tokens)
// {
// 	t_token	*rpn;
// 	t_token	*stack;
	
// }

void	parse_tokens(t_token **tokens)
{
	gen_ast_list(tokens);
	// rpn_tokens(tokens);
	// gen_ast(tokens);
}