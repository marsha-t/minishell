/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:54:46 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:54:54 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	create_arg allocates arg node and initialises it using arg_str*/
t_list	*create_arg(char *arg_str)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
	{
		err_printf("malloc error: t_list for matched_to_arg\n");
		return (NULL);
	}
	new->content = arg_str;
	new->next = NULL;
	return (new);
}

/*	convert_matched_to_arg converts matched list of directory contents
	into linked list of arguments
	- returns number of new arguments
	- returns -1 if malloc error
	- frees matched list  */
int	matched_to_arg(t_dconts *matched, t_list **new_args, t_list **end_args)
{
	t_dconts	*next_match;
	t_list		*new;
	int			count;

	count = 0;
	while (matched)
	{
		next_match = matched->next;
		new = create_arg(matched->cont_name);
		if (!new)
			return (free_conts_list(matched), -1);
		if (!*new_args)
			*new_args = new;
		else
			(*end_args)->next = new;
		*end_args = new;
		free(matched);
		matched = next_match;
		count++;
	}
	return (count);
}

/*	add_matched_to_arg inserts list of matched directory contents into args
	- updates node->n_args
	- returns 1 if matched_to_arg has malloc error */
int	add_matched_to_arg(t_dconts *matched, t_list **arg, t_ast *node)
{
	t_list	*new_args;
	t_list	*end_args;
	int		num_args;

	if (!matched)
		return (0);
	new_args = NULL;
	end_args = NULL;
	num_args = matched_to_arg(matched, &new_args, &end_args);
	if (num_args == -1)
		return (1);
	if (!*arg)
		*arg = new_args;
	else
	{
		end_args->next = (*arg)->next;
		(*arg)->next = new_args;
	}
	node->n_args += num_args;
	return (0);
}
