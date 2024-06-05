/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:01 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/03 06:15:11 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	create_node returns 1 for malloc error */
int create_node(t_var **v, char *str)
{
	t_var *new;
	t_var *current;
	char	*equal;
	
	new = (t_var *) malloc(sizeof(t_var));
	if(!new)
		return (ft_putstr_fd("Malloc error creating t_var in create_node\n", 2), 1);
	equal = ft_strchr(str, '=');
	new->key = strdup_range(str, equal - 1);
	if (str + ft_strlen(str) - 1 == equal)
		new->value = 0;
	else
		new->value = strdup_range(equal + 1, str + ft_strlen(str) - 1);
	new->next = NULL;
	if (!*v)
		*v = new;
	else
	{
		current = *v;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
	return (0);
}

t_var *create_list(char **envp)
{
	int i;
	t_var *v;
	
	v = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (create_node(&v, envp[i]) == 0)
			return (NULL);
		i++;
	}
	return(v);
}

/*	free_var_list frees all the nodes in the variable linked list
	and the key and values inside */
void	free_var_list(t_var *var)
{
	t_var	*current;
	t_var	*next;

	current = var;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
