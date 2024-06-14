/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:45:56 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/13 13:42:51 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// work in progress: these functions haven't been updated in case they are used in expansions 

t_var *check_exist(char *word, t_var *list)
{
	t_var *current;

	current = list;
	word = return_key(word);
	while(current && word)
	{
		if(search_for_key(word,current->content)!= NULL)
			return(free(word), current);
		current = current -> next;
	}
	return(free(word), NULL);
}

t_var *search_for_node(char *s2, t_var **list)
{
	t_var *current;

	current = *list;
	while(current)
	{
		if(search_for_key(s2, current -> content))
			return(current);
		current = current->next;
	}
	return(NULL);
}
