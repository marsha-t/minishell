/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_conts_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:41:27 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 07:41:43 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	create_conts_node creates t_dconts node using ft_strdup(str)
	and adds it to list 
	- returns 1 if malloc error*/
int	create_conts_node(char *str, t_dconts **list)
{
	t_dconts	*curr;
	t_dconts	*new;

	if (!str)
		return (1);
	curr = *list;
	new = ft_calloc(1, sizeof(t_dconts));
	if (!new)
		return (err_printf("minishell: malloc error: directory_contents\n"), 1);
	new->cont_name = ft_strdup(str);
	if (!new->cont_name)
		return (err_printf("minishell: malloc error: t_dconts->cont_name\n"), 1);
	new->next = NULL;
	if (!curr)
	{
		*list = new;
		return (0);
	}
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (0);
}

/*  order_conts_list uses bubble sort to order list in alphabetical order
	- in each pass of linked list, it swaps nodes that are out of order
	- algorithm ends when there is no swapping in the pass */
void	order_conts_list(t_dconts **list)
{
	int			swapped;
	t_dconts	*curr;
	char		*temp;

	while (1)
	{
		swapped = 0;
		curr = *list;
		while (curr->next != NULL)
		{
			if (ft_strcmp(curr->cont_name, curr->next->cont_name) > 0)
			{
				temp = curr->cont_name;
				curr->cont_name = curr->next->cont_name;
				curr->next->cont_name = temp;
				swapped = 1;
			}
			curr = curr->next;
		}
		if (swapped == 0)
			break ;
	}
}

/*	create_conts_list creates list of directory contents 
	to be stored in shell
	- sort list in alphabetical order 
	- returns 1 if error calling opendir or readdir 
		or malloc error in create_conts_node*/
t_dconts	*create_conts_list(char *directory)
{
	struct dirent	*content;
	DIR				*dd;
	t_dconts		*list;

	dd = opendir(directory);
	if (!dd)
		return (err_printf("minishell: error calling opendir\n"), NULL);
	list = NULL;
	content = readdir(dd);
	while (content)
	{
		if (create_conts_node(content->d_name, &list) == 1)
			return (NULL);
		content = readdir(dd);
	}
	closedir(dd);
	if (errno)
		return (err_printf("minishell: error calling readdir\n"), NULL);
	order_conts_list(&list);
	return (list);
}

/*	free_conts_list frees lists of directory contents */
void	free_conts_list(t_dconts *list)
{
	t_dconts	*current;
	t_dconts	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->cont_name);
		free(current);
		current = next;
	}
}

/*	print_conts_list prints files in list of directory contents*/
void	print_conts_list(t_dconts *list)
{
	t_dconts	*current;

	current = list;
	while (current)
	{
		ft_printf("file: %s\n", current->cont_name);
		current = current->next;
	}
}
