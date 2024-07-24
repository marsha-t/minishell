/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 18:53:05 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_exist checks whether a variable already exists
	- returns pointer to variable node if it does
	- returns NULL if not*/
t_var	*check_exist(char *word, t_var *list)
{
	t_var	*current;

	current = list;
	while (current)
	{
		if (ft_strcmp(current->key, word) == 0)
			return (current);
		current = current -> next;
	}
	return (NULL);
}

void	existing_var(t_var *exist, char	*value, char	*equal, char	*key)
{
	exist->env = 1;
	if (equal)
	{
		free(exist->value);
		exist->value = value;
		exist->flag = 1;
		exist->env = 1;
	}
	free(key);
}

int	non_existing_var(t_shell *shell, char *curr_content, char *equal)
{
	if (equal)
	{
		if (create_node(&shell->var_list, curr_content, 1))
			return (shell->exit_shell = 1, 1);
	}
	else
	{
		if (create_node(&shell->var_list, curr_content, 0))
			return (shell->exit_shell = 1, 1);
	}
	return (0);
}

/*builtin_export runs the export command
	- if no args, prints environment variable list
	- if there are args,
		- checks whether there is an equal sign
			- if there is no equal sign, variable is listed in export but not in env
		- checks whether key is a valid variable name
		- if key is invalid, returns error
		- if key already exists (as env or normal),
			- if no equal, change to env (if originally normal);
				original value is retained
			- if equal, replace in list
		- if key doesn't exist, create new node in list
			- if no equal, value = NULL
			- if equal, value = empty string
	- options are treated as invalid variable names*/
int	process_export_arg(t_list *curr_arg, t_shell *shell, int *exit_status)
{
	char	*key;
	char	*value;
	char	*equal;
	t_var	*exist;

	if (create_key_value(curr_arg->content, &equal, &key, &value) == 1)
		return (shell->exit_shell = 1, 1);
	if (valid_varname(key) == 1)
	{
		err_printf("export: `%s': not a valid identifier\n", key);
		free_num(2, key, value);
		*exit_status = 1;
		return (0);
	}
	exist = check_exist(key, shell->var_list);
	if (exist)
		existing_var(exist, value, equal, key);
	else
	{
		free_num(2, key, value);
		if (non_existing_var(shell, curr_arg->content, equal) == 1)
			return (1);
	}
	return (0);
}

int	builtin_export(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;
	int		exit_status;

	exit_status = 0;
	if (node->n_args == 0)
		return (print_export(shell->var_list), 0);
	curr_arg = node->args;
	while (curr_arg)
	{
		if (process_export_arg(curr_arg, shell, &exit_status) == 1)
			return (1);
		curr_arg = curr_arg->next;
	}
	return (exit_status);
}
