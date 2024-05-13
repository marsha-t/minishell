/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_misc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:53:07 by mateo             #+#    #+#             */
/*   Updated: 2024/05/13 14:08:04 by mateo            ###   ########.fr       */
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
char	*strdup_range(char *start, char *end)
{
	char	*ptr;
	size_t	n;
	int		i;

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
	ptr[i + 1] = '\0';
	return (ptr);
}

/*	is_file_op returns 1 for operators that require a file input: 
	i.e., >, >>, <, << */
int	is_file_op(int code)
{
	if (code == TOKEN_INPUT || code == TOKEN_OUTPUT \
		|| code == TOKEN_APPEND || code == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/*	is_cmdorder_op returns 1 for operators that affect 
	order in which commands are run: i.e., ||, &&, (
	- | (pipe) is also added included 
		though technically it doesn't affect command order */
int	is_cmdorder_op(int code)
{
	if (code == TOKEN_PIPE || code == TOKEN_OR || code == TOKEN_AND \
		|| code == TOKEN_OBRACKET)
		return (1);
	return (0);
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
