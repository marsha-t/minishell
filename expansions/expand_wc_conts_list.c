/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_conts_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:41:27 by mateo             #+#    #+#             */
/*   Updated: 2024/07/11 05:53:17 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	create_conts_node creates t_dconts node using str
	and adds it to list 
	- returns 1 if malloc error*/
// work in progress: check malloc protection and error message 
int create_conts_node(char *str, t_dconts **list)
{
    t_dconts *curr;
    t_dconts *new;

	if(!str)
		return (1);
    curr = *list;
    new = ft_calloc (1, sizeof(t_dconts));
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

/*	create_conts_list creates list of directory contents 
	to be stored in shell*/
// work in progress: check malloc protection and err msg
t_dconts *create_conts_list(void)
{
    struct dirent *content;
    DIR *dd;
    t_dconts *list;

    dd= opendir(".");
    list = ft_calloc(1, sizeof(t_dconts));
    if(!list)
     return (err_printf("minishell: malloc error: directory_contents\n", 2), NULL );
    content = readdir(dd);
    while (content)
    {
        if (create_conts_node(content->d_name, &list) == 1)
			return (NULL);
        content = readdir(dd);
    }
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
