/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_token_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:01:59 by mateo             #+#    #+#             */
/*   Updated: 2024/06/14 15:21:37 by mateo            ###   ########.fr       */
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
	if (!new)
		return (NULL);
	new->str = str;
	new->code = code;
	new->next = 0;
	return (new);
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
		return (ft_putstr_fd("Malloc error creating token->str\n", 2), 1);
	new = new_token(str, code);
	if (!new)
	{
		free(str);
		return (ft_putstr_fd("Malloc error creating token\n", 2), 1);
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
		free_num(2, current->str, current);
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
