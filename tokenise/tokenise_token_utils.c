/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_token_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:01:59 by mateo             #+#    #+#             */
/*   Updated: 2024/05/28 15:03:40 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/*	insert_token inserts a new token in the middle of list
	- new token contains str and code
	- new token inserted right after node
	- shifts token pointer to newly inserted token */
int	insert_token(t_token **node, char *str, int code)
{
	t_token	*next;
	t_token	*new;

	if (!str)
		return (1);
	new = new_token(str, code);
	if (!new)
	{
		free(str);
		return (1);
	}
	next = (*node)->next;
	(*node)->next = new;
	new->next = next;
	*node = (*node)->next;
	return (0);
}

/*	add_token adds a new token to end of list
	- new token contains str and code 
	- str in token is the address given (i.e., str given is not duplicated)
	- returns 1 if error creating token and frees str; 
	- returns 0 if successful */
int	add_token(t_token **tokens, char *str, int code)
{
	t_token	*current;
	t_token	*new;

	if (!str)
		return (1);
	new = new_token(str, code);
	if (!new)
	{
		free(str);
		return (1);
	}
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
	ft_printf("printing tokens: \n");
	if (!tokens)
		ft_printf("empty\n");
	while (tokens)
	{
		ft_printf("str: %s, code: %d\n", tokens->str, tokens->code);
		tokens = tokens->next;
	}
}
