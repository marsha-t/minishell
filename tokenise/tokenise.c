/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 06:47:40 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 06:05:54 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	tokenise_op tokenises operators (pipes and redirects)
	- returns rval = 1 if error adding token; 0 if successful */
int	tokenise_op(char **input, t_token **tokens)
{
	int	rval;

	rval = 0;
	if (**input == '|')
	{
		if (*(*input + 1) == '|')
		{
			rval = add_token(tokens, ft_strdup("||"), TOKEN_OR);
			(*input)++;
		}
		else
			rval = add_token(tokens, ft_strdup("|"), TOKEN_PIPE);
	}
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

/* tokenise_misc tokenises non-operators
	- tokens are separated by whitespace and operators
	- returns 1 if error adding token; 0 otherwise*/
int	tokenise_misc(char **input, t_token **tokens)
{
	int		quote;
	char	*start;

	quote = 0;
	start = *input;
	while (**input)
	{
		quote = check_quote(quote, **input);
		if (quote == 0 && ft_strchr(" \t|<>&()", **input))
			return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
		(*input)++;
	}
	return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
}

/*	check_syntax_tokens runs additional syntax checks:
	note: compound commands refer to those separated by &&, || or |
		simple commands refer to the commands that are between &&, || or |
	- simple commands cannot start with &&, || or |
	- redirection tokens must be followed by another TOKEN_TEMP
	- input cannot end with &&, ||, | or (
	- returns 1 if any of the above happens */
// work in progress: here, incomplete commands (ending with &&, || or |) are errors
int	check_syntax_tokens(t_token *tokens)
{
	t_token	*start;

	start = tokens;
	while (tokens)
	{
		if (tokens == start && is_cmdorder_op(tokens->code) == 1)
			return (err_printf("syntax error near unexpected token `%s'\n", tokens->str), 1);
		else if (is_file_op(tokens->code) && !tokens->next)
			return (err_printf("syntax error near unexpected token `newline'\n"), 1);
		else if (is_file_op(tokens->code) && tokens->next->code != TOKEN_TEMP)
			return (err_printf("syntax error near unexpected token `%s'\n", tokens->next->str), 1);
		else if (is_cmdorder_op(tokens->code) > 0)
		{
			start = tokens->next;
			if (!start)
				return (err_printf("syntax error near unexpected token `%s'\n", tokens->str), 1);
		}
		tokens = tokens->next;
	}
	return (0);
}

/*	sort_temp_tokens recategorises TOKEN_TEMP tokens
	note: compound commands refer to those separated by &&, || or |
		simple commands refer to the commands that are between &&, || or |
	- if redirection found: next token is a file
	- if no command tokens identified yet in simple command,
		first TOKEN_TEMP token is a command token
	- otherwise, all other tokens are arguments
	*/
void	sort_temp_tokens(t_token *tokens)
{
	int		cmd;

	cmd = 0;
	while (tokens)
	{
		if ((is_file_op(tokens->code) == 1) && \
			tokens->next->code == TOKEN_TEMP)
		{
			tokens->next->code = TOKEN_FILE;
			tokens = tokens->next;
		}
		else if (tokens->code == TOKEN_TEMP && cmd == 0)
		{
			tokens->code = TOKEN_CMD;
			cmd = 1;
		}
		else if (tokens->code == TOKEN_TEMP && cmd == 1)
			tokens->code = TOKEN_ARG;
		else if (is_cmdorder_op(tokens->code) == 1)
			cmd = 0;
		tokens = tokens->next;
	}
}

/*	tokenise creates linked list of tokens from input str
	- separate tokens in input str based on whitespace, pipe or redirect
	- categorises non-operator tokens into commands, arguments, files
	- returns 1 if error creating any of the tokens (tokens freed by main function)
	- checks syntax of tokens and returns 2 if syntax error*/
int	tokenise(char *input, t_token **tokens)
{
	while (*input)
	{
		while (ft_strchr(" \t", *input))
			input++;
		if (ft_strchr("|<>&()", *input))
		{
			if (tokenise_op(&input, tokens) == 1)
				return (1);
		}
		else
		{
			if (tokenise_misc(&input, tokens) == 1)
				return (1);
		}
	}
	if (check_syntax_tokens(*tokens) == 1)
		return (2);
	sort_temp_tokens(*tokens);
	return (0);
}
