/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:09:26 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:15:41 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_str_dquote(int *i, char **str, t_shell *shell)
{
	(*i)++;
	while ((*str)[*i] != '\0' && (*str)[*i] != '\"')
	{
		if ((*str)[*i] == '$' && is_valid_varstart((*str)[*i + 1]) == 0)
		{
			*str = split_expand_join(*str, *i, shell);
			if (!*str)
				return (1);
		}
		else
			(*i)++;
	}
	(*i)++;
	return (0);
}

int	expand_str_dollar(char **str, int *i, t_shell *shell)
{
	*str = split_expand_join(*str, *i, shell);
	if (!*str)
		return (1);
	*i = 0;
	return (0);
}

int	expand_str_while(char **str, int *i, t_shell *shell)
{
	if ((*str)[*i] == 39)
	{
		(*i)++;
		while ((*str)[*i] != '\0' && (*str)[*i] != 39)
			(*i)++;
		(*i)++;
	}
	else if ((*str)[*i] == '\"')
	{
		if (expand_str_dquote(i, str, shell) == 1)
			return (1);
	}
	else if ((*str)[*i] == '$' && is_quote((*str)[*i + 1]) == 0)
	{
		if (expand_str_dollar(str, i, shell) == 1)
			return (1);
	}
	else if ((*str)[*i] == '$' && is_valid_varstart((*str)[*i + 1]) == 0)
	{
		if (expand_str_dollar(str, i, shell) == 1)
			return (1);
	}
	else
		(*i)++;
	return (0);
}

/*	expand_str expands $
	- returns 1 if error
	- restarts at i = 0 after expanding to cater for nested expansions */
char	*expand_str(char *str, t_shell *shell)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (expand_str_while(&str, &i, shell) == 1)
			return (NULL);
	}
	return (str);
}
