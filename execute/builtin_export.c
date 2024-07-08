/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/07 15:54:58 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	print_export prints the environment variables 
	(prompted by export command)*/
void print_export(t_var *envp)
{
	t_var *current;

	current = envp;
	while (current)
	{
		if (current->env == 1)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, current->key, ft_strlen(current->key));
			if (current->value)
			{
				write(STDOUT_FILENO, "=\"", 2);
				write(STDOUT_FILENO, current->value, ft_strlen(current->value));
				write(STDOUT_FILENO, "\"", 1);
			}
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}

/*	check_exist checks whether a variable already exists
	- returns pointer to variable node if it does
	- returns NULL if not*/
t_var *check_exist(char *word, t_var *list)
{
	t_var *current;

	current = list;
	while (current)
	{
		if (ft_strcmp(current->key, word) == 0)
			return (current);
		current = current -> next;
	}
	return (NULL);
}

/*	builtin_export runs the export command
	- if no args, prints environment variable list 
	- if there are args, 
		- checks whether there is an equal sign
			- if there is no equal sign, variable is listed in export but not in env
		- checks whether key is a valid variable name
		- if key is invalid, returns error
		- if key already exists (as env or normal), 
			- if no equal, change to env (if originally normal); original value is retained
			- if equal, replace in list
		- if key doesn't exist, create new node in list 
			- if no equal, value = NULL
			- if equal, value = empty string
	- options are treated as invalid variable names
*/
int builtin_export(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;
	char	*key;
	char	*value;
	char	*equal;
	t_var	*exist;
	int	exit_status;

	exit_status = 0;
	if (node->n_args == 0)
		return (print_export(shell->var_list), 0);
	else
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (create_key_value(curr_arg->content, &equal, &key, &value) == 1)
			{
				shell->exit_shell = 1;
				return (1);
			}
			if (valid_varname(key) == 1)
			{
				err_printf("minishell: export: `%s': not a valid identifier\n", key);
				free_num(2, key, value);
				exit_status = 1;
				curr_arg = curr_arg->next;
				continue;
			}
			exist = check_exist(key, shell->var_list);
			if (exist)
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
			else
			{
				free_num(2, key, value);
				if (equal)
				{
					if (create_node(&shell->var_list, curr_arg->content, 1) == 1)
					{
						shell->exit_shell = 1;
						return (1); 
					}
				}
				else
				{
					if (create_node(&shell->var_list, curr_arg->content, 0) == 1)
					{
						shell->exit_shell = 1;
						return (1);
					}
				}
			}
			curr_arg = curr_arg->next;
		}
	}
	return (exit_status);
}
