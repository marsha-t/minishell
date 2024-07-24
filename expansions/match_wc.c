/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_wc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:40:07 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 16:01:21 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	match_pattern_str_quote(char **pattern, char **str)
{
	(*pattern)++;
	while (**pattern != '\"')
	{
		if (**pattern == **str)
		{
			(*pattern)++;
			(*str)++;
		}
		else
			return (1);
	}
	(*pattern)++;
	return (0);
}

int	match_pattern_str_star(char **pattern, char **str)
{
	(*pattern)++;
	if (**pattern == '\0')
		return (0);
	while (**str)
	{
		if (match_pattern_str(*pattern, (*str)++) == 0)
			return (0);
	}
	return (1);
}

/*	match_pattern_str checks whether str matches pattern
	- returns 0 if match; 1 otherwise */
int	match_pattern_str(char *pattern, char *str)
{
	if (!str)
		return (1);
	while (*pattern)
	{
		if (*pattern == '\"')
		{
			if (match_pattern_str_quote(&pattern, &str) == 1)
				return (1);
		}
		else if (*pattern == '*')
			return (match_pattern_str_star(&pattern, &str));
		else if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else
			return (1);
	}
	return (!(*str == '\0'));
}

/*	match_pattern_list iterates through list and checks for matches
	- matched nodes are duplicated and added to a linked list 
	- returns -1 if malloc error
	- otherwise returns number of matched cases */
int	match_pattern_list(char *pattern, t_dconts *list, t_dconts **matched_list)
{
	t_dconts	*current;
	int			count;

	current = list;
	count = 0;
	*matched_list = NULL;
	while (current)
	{
		if (match_pattern_str(pattern, current->cont_name) == 0)
		{
			count++;
			if (create_conts_node(current->cont_name, matched_list) == 1)
				return (-1);
		}
		current = current->next;
	}
	return (count);
}
