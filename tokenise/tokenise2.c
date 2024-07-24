/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:34:58 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 14:39:16 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tokenise_pipe(char **input, t_token **tokens)
{
	int	rval;

	if (*(*input + 1) == '|')
	{
		rval = add_token(tokens, ft_strdup("||"), TOKEN_OR);
		(*input)++;
	}
	else
		rval = add_token(tokens, ft_strdup("|"), TOKEN_PIPE);
	return (rval);
}

int	tokenise_redirects(char **input, t_token **tokens)
{
	int	rval;

	rval = 0;
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			rval = add_token(tokens, ft_strdup("<<"), TOKEN_HEREDOC);
			(*input)++;
		}
		else
			rval = add_token(tokens, ft_strdup("<"), TOKEN_INPUT);
	}
	else if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			rval = add_token(tokens, ft_strdup(">>"), TOKEN_APPEND);
			(*input)++;
		}
		else
			rval = add_token(tokens, ft_strdup(">"), TOKEN_OUTPUT);
	}
	return (rval);
}

/*	tokenise_op tokenises operators (pipes and redirects)
	- returns rval = 1 if error adding token; 0 if successful */
int	tokenise_op(char **input, t_token **tokens)
{
	int	rval;

	rval = 0;
	if (**input == '|')
		rval = tokenise_pipe(input, tokens);
	else if (**input == '<')
		rval = tokenise_redirects(input, tokens);
	else if (**input == '>')
		rval = tokenise_redirects(input, tokens);
	else if (**input == '&' && *(*input + 1) == '&')
	{
		rval = add_token(tokens, ft_strdup("&&"), TOKEN_AND);
		(*input)++;
	}
	else if (**input == '(')
		rval = add_token(tokens, ft_strdup("("), TOKEN_OBRACKET);
	else if (**input == ')')
		rval = add_token(tokens, ft_strdup(")"), TOKEN_CBRACKET);
	else
		rval = add_token(tokens, strdup_range(*input, *input + 1), TOKEN_TEMP);
	(*input)++;
	return (rval);
}
