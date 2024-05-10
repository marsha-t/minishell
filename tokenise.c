/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 06:47:40 by mateo             #+#    #+#             */
/*   Updated: 2024/05/10 09:59:49 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*	strdup_range duplicates str based on start and end pointers*/
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
	- str is an allocated str*/
t_token	*new_token(char *str, int code)
{
	t_token	*new;
	
	new = malloc(sizeof(t_token));
	new->str = str;
	if (!new->str)
	{
		free(new); // error
		return (0);
	}
	new->code = code;
	new->next = 0;
	return (new);
}

/*	add_token adds a new token to end of list
	- new token contains str and code */
void	add_token(t_token **tokens, char *str, int code)
{
	t_token	*current;
	
	if (!*tokens)
		*tokens = new_token(str, code);
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = new_token(str, code);
	}
}

/*	tokenise_op tokenises operators (pipes and redirects)*/
void	tokenise_op(char **input, t_token **tokens)
{
	if (**input == '|')
		add_token(tokens, ft_strdup("|"), TOKEN_PIPE);
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			add_token(tokens, ft_strdup("<<"), TOKEN_HEREDOC);
			(*input)++;
		}
		else
			add_token(tokens, ft_strdup("<"), TOKEN_INPUT);
	}
	else if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			add_token(tokens, ft_strdup(">>"), TOKEN_APPEND);
			(*input)++;
		}
		else
			add_token(tokens, ft_strdup(">"), TOKEN_OUTPUT);
	}
	(*input)++;
}

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

/* tokenise_misc tokenises non-operators
	- tokens are separated by whitespace*/
void	tokenise_misc(char **input, t_token **tokens)
{
	int	quote;
	char *start;
	
	quote = 0;
	start = *input;
	while (**input)
	{
		quote = check_quote(**input);
		if (quote == 0 && ft_strchr(" \t\n|<>", **input))
			return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
		(*input)++;
	}
	add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP);
}

int	sort_temp_tokens(t_token *tokens) // use this to check for further syntax issues/assumed order/ add check at start
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

/*	tokenise creates linked list of tokens from input
	- tokens separated by whitespace, pipe or redirect */
t_token	*tokenise(char *input) // double pointer to change input?
{
	t_token	*tokens;

	tokens = 0;
	input = ft_strtrim(input, " \t");
	while (*input)
	{
		while (ft_strchr(" \t", *input))
			input++;
		if (ft_strchr("|<>", *input))
			tokenise_op(&input, &tokens);
		else
			tokenise_misc(&input, &tokens);
	}
	sort_temp_tokens(tokens);
	return (tokens);
}

void	print_token(t_token *tokens)
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

// int main(void)
// {
// 	// print_token(tokenise(ft_strdup("| > <")));
// 	// print_token(tokenise(ft_strdup("echo \"abc\" | cat")));
// 	// print_token(tokenise(ft_strdup("echo \"abc\" | cat ")));
// 	// print_token(tokenise(ft_strdup("echo \"abc\" | cat\t")));
// 	// print_token(tokenise(ft_strdup("echo \"abc\" | cat\t < input.txt"))); 
// 	// print_token(tokenise(ft_strdup("ech\"o\" \"abc\" | cat\t"))); // can add flag to node to remove quote

// 	// print_token(tokenise(ft_strdup("cat << eof this is line 1 \n this is line 2\n eof"))); 

// 	// printf("ori: .%s.\ntrimmed: .%s.\n", ft_strdup(" ab "), ft_strtrim(ft_strdup(" ab\t"), "\t "));
// 	// printf("ori: .%s.\ntrimmed: .%s.\n", ft_strdup("  cat\t "), ft_strtrim(ft_strdup("  cat\t "), "\t "));

// 	return (0);
// }