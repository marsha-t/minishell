/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 06:47:40 by mateo             #+#    #+#             */
/*   Updated: 2024/05/10 15:04:10 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	ft_strcmp compares two strings
	- returns non-zero if they are different;
	- returns 0 if they are the same */
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*	strdup_range duplicates str based on start and end pointers
	- function needed to add non-operators to token list */
char *strdup_range(char *start, char *end)
{
	char *ptr;
	size_t	n;
	int i;

	n = end - start;
	ptr = malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = 0;
	while (start != end)
	{
		ptr[i] = *start;
		start++;
		i++;
	}
	ptr[i] = *end;
	ptr[i+1] = '\0';
	return (ptr);
}

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

/*	tokenise_op tokenises operators (pipes and redirects)
	- returns rval = 1 if error adding token; 0 if successful */
int	tokenise_op(char **input, t_token **tokens)
{
	int rval;

	rval = 0;
	if (**input == '|')
		rval = add_token(tokens, ft_strdup("|"), TOKEN_PIPE);
	else if (**input == '<')
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
	(*input)++;
	return (rval);
}

/* tokenise_misc tokenises non-operators
	- tokens are separated by whitespace and operators
	- returns 1 if error adding token; 0 otherwise*/
int	tokenise_misc(char **input, t_token **tokens)
{
	int	quote;
	char *start;
	
	quote = 0;
	start = *input;
	while (**input)
	{
		quote = check_quote(**input);
		if (quote == 0 && ft_strchr(" \t|<>", **input))
			return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
		(*input)++;
	}
	return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
}

int	sort_temp_tokens(t_token *tokens)
{
	t_token	*start;

	start = tokens;
	while (tokens)
	{
		if ((tokens == start) && tokens->code == TOKEN_TEMP)
			tokens->code = TOKEN_CMD;
		else if (tokens->code == TOKEN_PIPE &&  \
			tokens->next->code == TOKEN_TEMP)
			tokens->next->code = TOKEN_CMD;
		if ((tokens->code == TOKEN_CMD || \
			tokens->code == TOKEN_ARG) && \
			tokens->next && tokens->next->code == TOKEN_TEMP)
			tokens->next->code = TOKEN_ARG;
		if ((tokens->code == TOKEN_INPUT || \
			tokens->code == TOKEN_OUTPUT || \
			tokens->code == TOKEN_APPEND) && \
			tokens->next->code == TOKEN_TEMP)
			tokens->next->code = TOKEN_FILE;
		tokens = tokens->next;
	}
	return (0);
}

/*	tokenise creates linked list of tokens from input str
	- separate tokens in input str based on whitespace, pipe or redirect
	- whitespaces in input str  */
t_token	*tokenise(char *input)
{
	t_token	*tokens;

	tokens = 0;
	input = ft_strtrim(input, " \t");
	while (*input)
	{
		while (ft_strchr(" \t", *input))
			input++;
		if (ft_strchr("|<>", *input))
		{
			if (tokenise_op(&input, &tokens) == 1)
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (tokenise_misc(&input, &tokens) != 0);
				return (free_tokens(tokens), NULL);
		}
	}
	sort_temp_tokens(tokens);
	return (tokens);
}

// int main(void)
// {
// 	t_token *tokens;

// 	tokens = tokenise(line);
// 	if (!tokens) // error generating tokens
// 		return (1);
// }
