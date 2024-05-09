/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 06:47:40 by mateo             #+#    #+#             */
/*   Updated: 2024/05/09 08:40:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>

# define TOKEN_TEMP 0
# define TOKEN_CMD 1
# define TOKEN_ARG 2
# define TOKEN_PIPE 3
# define TOKEN_INPUT 4
# define TOKEN_OUTPUT 5
# define TOKEN_APPEND 6
# define TOKEN_HEREDOC 7
# define TOKEN_FILE 8


typedef struct	s_token
{
	char			*str;
	int				token_code;
	struct s_token	*next;
}	t_token;


int ft_strlen(const char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}

/// strtrim

static int	ft_is_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static size_t	ft_strtrimlen(char *s1, char *set) // remove const
{
	int	len;

	while (ft_is_set(*s1, set))
		s1++;
	if (*s1 == '\0')
		return (0);
	len = 0;
	while (*s1)
	{
		len++;
		s1++;
	}
	s1--;
	while (ft_is_set(*s1, set))
	{
		len--;
		s1--;
	}
	if (len < 0)
	{
		len = 0;
	}
	return ((size_t)len);
}

char	*ft_strtrim(char *s1, char *set) // remove const
{
	size_t	len;
	size_t	i;
	char	*ptr;
	char	*ori; // new

	if ((!s1) || (!set))
		return (0);
	ori = s1;
	len = ft_strtrimlen(s1, set);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (0);
	while (ft_is_set(*s1, set))
		s1++;
	i = 0;
	while ((len--) && (*s1))
	{
		ptr[i] = *s1;
		i++;
		s1++;
	}
	ptr[i] = '\0';
	free(ori);// new
	return (ptr);
}
///
char	*ft_strdup(const char *src)
{
	char		*ptr;
	size_t		n;
	int			i;

	n = ft_strlen(src);
	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = -1;
	while (src[++i] != '\0')
		ptr[i] = src[i];
	ptr[i] = '\0';
	return (ptr);
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
	new->token_code = code;
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
		if (quote == 0 && ft_strchr(" \t|<>", **input))
			return (add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP));
		(*input)++;
	}
	add_token(tokens, strdup_range(start, (*input) - 1), TOKEN_TEMP);
}

/*	tokenise creates linked list of tokens from input
	- tokens separated by whitespace, pipe or redirect*/
t_token	*tokenise(char *input) // double pointer to change input?
{
	t_token	*tokens;

	tokens = 0;
	input = ft_strtrim(input, " \t");
	while (*input)
	{
		while (ft_strchr(" \t", *input)) // to add new line for heredoc?
			input++;
		printf("%c, %d\n", *input, *input);
		if (ft_strchr("|<>", *input))
			tokenise_op(&input, &tokens);
		else
			tokenise_misc(&input, &tokens);
			// input++;
	}
	return (tokens);
}

void	print_token(t_token *tokens)
{
	if (!tokens)
		printf("empty\n");
	while (tokens)
	{
		printf("str: %s, code: %d\n", tokens->str, tokens->token_code);
		tokens = tokens->next;
	}
}

int main(void)
{
	// print_token(tokenise(ft_strdup("| > <")));
	// print_token(tokenise(ft_strdup("echo \"abc\" | cat")));
	// print_token(tokenise(ft_strdup("echo \"abc\" | cat ")));
	// print_token(tokenise(ft_strdup("echo \"abc\" | cat\t")));
	print_token(tokenise(ft_strdup("echo \"abc\" | cat\ta"))); 
	// print_token(tokenise(ft_strdup("ech\"o\" \"abc\" | cat\t"))); // can add flag to node to remove quote


	// printf("ori: .%s.\ntrimmed: .%s.\n", ft_strdup(" ab "), ft_strtrim(ft_strdup(" ab\t"), "\t "));
	// printf("ori: .%s.\ntrimmed: .%s.\n", ft_strdup("  cat\t "), ft_strtrim(ft_strdup("  cat\t "), "\t "));

	return (0);
}