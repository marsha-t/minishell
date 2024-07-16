/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:01 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/16 18:46:15 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	create_node returns 1 for malloc error */
int	create_node(t_var **v, char *str, int flag)
{
	t_var	*new;
	t_var	*current;
	char	*equal;

	new = malloc(sizeof(t_var));
	if (!new)
		return (err_printf("minishell: malloc error: t_var in create_node\n"), 1);
	if (create_key_value(str, &equal, &new->key, &new->value) == 1)
		return (1);
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
	return (0);
}

t_var	*create_list(char **envp)
{
	int		i;
	t_var	*v;

	v = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (create_node(&v, envp[i], 1) == 1)
			return (NULL);
		i++;
	}
	return (v);
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
		if (current->key)
		{
			free(current->key);}
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	var = NULL;
}
