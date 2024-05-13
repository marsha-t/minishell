/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation_token_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:01:59 by mateo             #+#    #+#             */
/*   Updated: 2024/05/13 14:07:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	new_token generates a new token
	- str is an allocated str 
	- returns null pointer if malloc error */
t_token	*new_token(char *str, int code)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->str = str;
	if (!new->str)
		return (NULL);
	new->code = code;
	new->next = 0;
	return (new);
}

/*	add_token adds a new token to end of list
	- new token contains str and code 
	- str in token is the address given (i.e., str given is not duplicated)
	- returns 1 if error creating token; 0 if successful */
int	add_token(t_token **tokens, char *str, int code)
{
	t_token	*current;
	t_token	*new;

	new = new_token(str, code);
	if (!new)
		return (1);
	if (!*tokens)
		*tokens = new;
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

/*	free_tokens frees nodes in tokens linked list 
	and the str inside the nodes */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(tokens->str);
		free(current);
		current = next;
	}
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
