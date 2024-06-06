/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/06/06 05:36:19 by mateo            ###   ########.fr       */
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
		*key = strdup_range(str, *equal - 1);
		if (!*key)
			return (ft_putstr_fd("Malloc error creating key", 2), 1);
		if (*equal + 1 == '\0')
		{
			*value = ft_strdup("");
			if (!*value)
			{
				free(*key);
				return (ft_putstr_fd("Malloc error creating value", 2), 1);
			}
		}
		else
		{
			*value = strdup_range(*equal + 1, str + ft_strlen(str));
			if (!*value)
			{
				free(*key);
				return (ft_putstr_fd("Malloc error creating value", 2), 1);
			}
		}
	}
	else
	{
		*key = ft_strdup(str);
		if (!*key)
			return (ft_putstr_fd("Malloc error creating key\n", 2), 1);
		*value = 0;
	}
	return (0);
}

/*	create_node_normal creates a node for a normal variable 
	and adds it to the end of shell->var_list*/
int	create_node_normal(t_var **v, char *key, char *value)
{
	t_var	*new;
	t_var	*current;

	new = malloc(sizeof(t_var));
	if (!new)
		return (ft_putstr_fd("Malloc error creating t_var\n", 2), 1);
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

/*	run_assign_str runs the assignment given (either in cmd or arg)
	- if key name already exists, replace value
	- else checks if key name is valid 
		if yes, new key added to end of linked list
		if not, error msg */
int	run_assign_str(char *cmd, t_shell *shell)
{
	char	*equal;
	char	*key;
	char	*value;
	t_var	*exist;
	
	if (create_key_value(cmd, &equal, &key, &value) == 1)
		return (1);
	if (valid_varname(key) == 1)
	{
		free(key);
		free(value);
		return ((ft_putstr_fd("Invalid variable name\n", 2), 1);
	}
	exist = check_exist(key, shell->var_list);
	if (exist)
	{
		free(exist->value);
		exist->value = value;
		free(key);
	}
	else 
	{
		if (create_node_normal(&shell->var_list, key, value) == 1)
		{
			free(key);
			free(value);
			return (1);
		}
		free(key);
	}
}

/*	check_assign_varname checks that key in assignments is valid
	- checks cmd and args (only args that have the equal sign) */
int	check_assign_varname(t_ast *node)
{
	char	*equal;
	char	*key;
	int		i;
	
	equal = ft_strchr(node->cmd, '=');
	key = strdrup_range(node->cmd, equal - 1);
	if (!key)
		return (ft_putstr_fd("Malloc error creating key for check_assign_varname\n", 2), 1);
	if (valid_varname(key) == 1)
		return (ft_putstr_fd("Invalid variable name provided", 2), 1);
	free(key);
	if (node->n_args > 0)
	{
		i = 0;
		while (i < node->n_args)
		{
			equal = ft_strchr(node->args[i], '=');
			if (equal)
			{
				key = strdup_range(node->args[i], equal - 1);
				if (!key)
					return (ft_putstr_fd("Malloc error creating key for check_assign_varname\n", 2), 1);
				if (valid_varname(key) == 1)
					return (ft_putstr_fd("Invalid variable name provided", 2), 1);
				free(key);
			}
			else
				break;
			i++;
		}
	}
	return (0);	
}

/*	run_assign_cmd runs the remaining args in assignment as a simple command
*/
// work in progress
int	run_assign_cmd(t_ast *node, t_shell *shell, int i)
{
	while (i < node->n_args)
	{
		i++;
	}
}

/*	run_assign runs the assignments in cmd and args (if any) 
	- returns 1 if failure (malloc) */
// work in progress: finish run_assign_cmd
int	run_assign(t_ast *node, t_shell *shell)
{
	int		i;
	char	*equal;

	if (check_assign_varname(node) == 1)
		return (1);
	if (run_assign_str(node->cmd, shell) == 1)
		return (1);
	if (node->n_args > 0)
	{
		i = 0;
		while (i < node->n_args)
		{
			if (ft_strchr(node->args[i], '='))
			{
				if (run_assign_str(node->args[i]) == 1)
					return (1);
			}
			else
				break;
			i++;
		}
		if (i < node->n_args)
		{
			return (run_assign_cmd(node, shell, i));
		}
	}
	return (0);
}
