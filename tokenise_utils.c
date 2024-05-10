/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:53:07 by mateo             #+#    #+#             */
/*   Updated: 2024/05/10 14:55:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*	check_quote returns 1 if input is single quote;
	2 for double quote;
	0 for no quote*/
int	check_quote(char input)
{
	if (input == '\'')
		return (1);
	else if (input == '\"')
		return (2);
	else
		return (0);
}

/*	print_token prints info in the token linked list
	- used for debugging */
void	print_tokens(t_token *tokens)
{
	printf("printing tokens: \n");
	if (!tokens)
		printf("empty\n");
	while (tokens)
	{
		printf("str: %s, code: %d\n", tokens->str, tokens->code);
		tokens = tokens->next;
	}
}

/*	free_tokens frees nodes in tokens linked list 
	and the str inside the nodes */
void	free_tokens(t_token *tokens)
{
	t_token *current;
	t_token *next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(token->str);
		free(current);
		current = next;
	}
}
