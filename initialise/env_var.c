/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:01 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/05 13:40:57 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int create_node(t_var **v, char *str, int flag)
{
	t_var *new;
	t_var *current;
	char	*equal;
	
	new = (t_var *) malloc(sizeof(t_var));
	if(!new)
		return(ft_putstr_fd("Malloc error creating t_var", 2), 0);
	equal = ft_strchr(str, '=');
	new->key = strdup_range(str, equal - 1);
	if (!new->key)
		return (ft_putstr_fd("Malloc error creating t_var->key\n", 2), 0);
	if (equal + 1 == '\0')
	{
		new->value = ft_stdup("");
		if (!new->value)
			return (ft_putstr_fd("Malloc error creating t_var->value\n", 2), 0);
	}
	else
	{
		new->value = strdup_range(equal + 1, str + ft_strlen(str) - 1);
		if (!new->value)
			return (ft_putstr_fd("Malloc error creating t_var->value\n", 2), 0);
	}
	new->flag = flag;
	new->env = 1;
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
	return (1);
}

t_var *create_list(char **envp)
{
	int i;
	t_var *v;
	
	v = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (create_node(&v, envp[i], 1) == 0)
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
