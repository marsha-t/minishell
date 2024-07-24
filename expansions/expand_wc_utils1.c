/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:38:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:54:39 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_dot_wc(char *pattern)
{
	if (pattern[0] == '.')
		return (0);
	else if (ft_strncmp(pattern, "\".", 2) == 0)
		return (0);
	else if (ft_strncmp(pattern, "\'.", 2) == 0)
		return (0);
	return (1);
}

void	rm_dot_dconts(t_dconts **list)
{
	t_dconts	*curr;
	t_dconts	*temp;
	t_dconts	*prev;

	curr = *list;
	prev = NULL;
	while (curr)
	{
		if (curr->cont_name[0] == '.')
		{
			if (!prev)
				*list = curr->next;
			else
				prev->next = curr->next;
			temp = curr;
			curr = curr->next;
			free(temp->cont_name);
			free(temp);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

/*	count_matches counts number of nodes
	in t_dconts linked list */
int	count_matches(t_dconts *matched)
{
	int	count;

	count = 0;
	while (matched)
	{
		count++;
		matched = matched->next;
	}
	return (count);
}

/*	init_wc allocates and initialises wc_info */
int	init_wc(t_wc **wc_info, char *cmd)
{
	*wc_info = malloc(sizeof(t_wc));
	if (!*wc_info)
		return (err_printf("malloc error: wc_info\n"), 1);
	(*wc_info)->matched = NULL;
	(*wc_info)->cmd = cmd;
	return (0);
}

/*	free_wc_info frees matched linked list as well as wc struct */
void	free_wc_info(t_wc *wc_info)
{
	free_conts_list(wc_info->matched);
	free(wc_info);
}
