/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 18:41:55 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	valid_varname checks whether a name is valid
	- only contains letters, digits or _
	- cannot start with a digit
	- returns 0 if name is valid; 1 otherwise */

int	valid_varname(char *name)
{
	int	i;

	i = 0;
	if (name == NULL)
		return (1);
	while (name[i])
	{
		if (ft_isalnum(name[i]) == 1 || name[i] == '_')
			i++;
		else
			return (1);
	}
	if (ft_isdigit(name[0]) == 1)
		return (1);
	return (0);
}

/*	create_key_value splits str into key, equal and value */
int	create_key_value(char *str, char **equal, char **key, char **value)
{
	*equal = ft_strchr(str, '=');
	if (*equal)
	{
		if (*equal == str)
		{
			*key = 0;
			*value = 0;
		}
		else
		{
			if (extract_key_value(str, equal, key, value) == 1)
				return (1);
		}
	}
	else
	{
		*key = ft_strdup(str);
		if (!*key)
			return (err_printf("malloc error: key\n"), 1);
		*value = 0;
	}
	return (0);
}

/*	create_node_normal creates a node for a normal variable
	(i.e., env = 0)
	and adds it to the end of shell->var_list*/
int	create_node_normal(t_var **v, char *key, char *value)
{
	t_var	*new;
	t_var	*current;

	new = malloc(sizeof(t_var));
	if (!new)
		return (err_printf("malloc error: t_var\n"), 1);
	new->key = key;
	new->value = value;
	new->env = 0;
	new->flag = 0;
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

int	check_value(char *equal, char *key, t_shell *shell, t_list	*curr_arg)
{
	while (curr_arg)
	{
		equal = ft_strchr(curr_arg->content, '=');
		if (equal)
		{
			key = strdup_range(curr_arg->content, equal - 1);
			if (!key)
			{
				shell->exit_shell = 1;
				err_printf("malloc error: key for check_assign_varname\n");
				return (1);
			}
			if (valid_varname(key) == 1)
			{
				err_printf("%s: command not found\n", curr_arg->content);
				return (1);
			}
			free(key);
		}
		else
			break ;
		curr_arg = curr_arg->next;
	}
	return (0);
}

/*	run_assign runs the assignments in cmd and args (if any)
	- run_assign_str adds variables into shell->var_list
		- each arg node is freed after each addition into var_list
	- returns 1 if failure (malloc)
	- if there are remaining args after assignment, ast node is configured
		so first arg is a cmd
	- otherwise, return 0 */
int	run_assign(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	if (check_assign_varname(node, shell) == 1
		|| run_assign_str(node->cmd, shell) == 1)
		return (1);
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (ft_strchr(curr_arg->content, '='))
			{
				if (run_assign_str(curr_arg->content, shell) == 1)
					return (1);
				node->args = curr_arg->next;
				(free(curr_arg->content), free(curr_arg));
				curr_arg = node->args;
			}
			else
				break ;
		}
		if (curr_arg)
			return (run_assign_cmd(node, shell));
	}
	return (0);
}
