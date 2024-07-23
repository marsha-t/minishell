/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:50:17 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 14:52:39 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	remove_quote_inquote(int *start, int *i, char *str, char **in_quote)
{
	if (*start == *i)
		*in_quote = ft_strdup("");
	else
		*in_quote = strdup_range(&str[*start], &str[*i - 1]);
	if (!*in_quote)
		return (err_printf("malloc error: in_quote\n"), 1);
	return (0);
}

/*	remove_quote_join splits str into what is inside the quote and after it
	then combines it
	- updates start and i as well */
char	*remove_quote_join(char *str, char *before_quote, int *start, int *i)
{
	char	*in_quote;
	char	*after_quote;
	char	*combined;

	if (remove_quote_inquote(start, i, str, &in_quote) == 1)
		return (NULL);
	if (*i == (int)ft_strlen(str) - 1)
		after_quote = ft_strdup("");
	else
		after_quote = strdup_range(&str[*i + 1], &str[ft_strlen(str) - 1]);
	if (!after_quote)
	{
		free(in_quote);
		return (err_printf("malloc error: after_quote\n"), NULL);
	}
	free(str);
	*i = ft_strlen(before_quote) + ft_strlen(in_quote) - 1;
	combined = strjoin_num_free(3, before_quote, in_quote, after_quote);
	if (!combined)
		return (free_num(3, before_quote, in_quote, after_quote), NULL);
	*start = *i;
	return (combined);
}

int	remove_quote_str_init(char *str, char *quote, int *i, int *start)
{
	if (!str)
		return (err_printf("malloc error: ft_strdup\n"), 1);
	*quote = 0;
	*i = 0;
	*start = 0;
	return (0);
}

int	remove_quote_str_quoted(int i, char **before_quote, char *str, int *start)
{
	if (i == 0)
		*before_quote = ft_strdup("");
	else
		*before_quote = strdup_range(&str[0], &str[i - 1]);
	if (!*before_quote)
		return (err_printf("malloc error: before_quote\n"), 1);
	*start = i + 1;
	return (0);
}

int	remove_quote_str_while(char **str, int i, char quote, int *start)
{
	char	*before_quote;

	while ((*str)[i])
	{
		if ((*str)[i] == 34 || (*str)[i] == 39)
		{
			if (quote == 0)
			{
				quote = (*str)[i];
				if (remove_quote_str_quoted(i, &before_quote, *str, start) == 1)
					return (1);
			}
			else if (quote == (*str)[i])
			{
				*str = remove_quote_join(*str, before_quote, start, &i);
				if (!*str)
					return (1);
				quote = 0;
			}
		}
		i++;
	}
	return (0);
}
