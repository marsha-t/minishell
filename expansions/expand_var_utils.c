/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:57:29 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 07:12:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	is_valid_varstart checks whether the char after $ is valid
	- valid if c is a letter, underscore, or question mark: returns 0 */
int	is_valid_varstart(char c)
{
	if (ft_isalpha(c) == 1)
		return (0);
	else if (c == '_')
		return (0);
	else if (c == '?')
		return (0);
	else
		return (1);
}

/*	is_valid_varchar returns 0 if char is valid character for a variable
	- valid if c is a letter, number or underscore*/
int	is_valid_varchar(char c)
{
	if (ft_isalpha(c) == 1)
		return (0);
	else if (ft_isdigit(c) == 1)
		return (0);
	else if (c == '_')
		return (0);
	return (1);
}

/*	is_quote returns 0 if c is a single or double quote */
int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}

/*	contain_var checks whether str contains a variable expansion 
	i.e., a $ followed by valid variable name character 
	- if $ is followed by a starting quote, 
	it is counted as a variable expansion (that expands into empty str)
	- if $ followed by number of special character, 
	it doesn't count as variable expansion
	- if $ is inside single quotes, it isn't a variable expansion
	- if there is variable expansion, return (0)
*/
int	contain_var(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39)
		{
			i++;
			while (str[i] != '\0' && str[i] != 39)
				i++;
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\0' && str[i] != '\"')
			{
				if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
					return (0);
				i++;
			}
		}
		else if (str[i] == '$' && is_quote(str[i + 1]) == 0)
			return (0);
		else if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
			return (0);
		i++;
	}
	return (1);
}
