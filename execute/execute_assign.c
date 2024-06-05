/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/06/05 14:06:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// work in progress: update for new variable list structure
// work in progress: assignment for an env var leaves it as an env var

/*	valid_varname checks whether a name is valid 
	- only contains letters, digits or _
	- cannot start with a digit
	- returns 1 if name is valid; 0 otherwise */
int	valid_varname(char *name)
{
	int	i;

	i = 0;
	while (name[i])
	{
		if (ft_isalnum(name[i]) == 1 || name[i] == '_')
			i++;
		else
			return (0);
	}
	if (ft_isdigit(name[0]) == 1)
		return (0);
	return (1);
}

/*	run_assign_str runs the assignment given (either in cmd or arg)
	- if key name already exists, replace value
	- else checks if key name is valid 
		if yes, new key added to end of linked list
		if not, error msg */
// work in progress pending structure of variable linked list and shell
// work in progress think thorugh interactions with export/env vars
int	run_assign_str(char *cmd, t_shell *shell)
{
	int		equal;
	char	*key;
	char	*value;
	t_var	*exist;
	
	equal = ft_strchr(cmd, '=');
	key = strdup_range(cmd, equal - 1);
	if (!key)
		return (ft_putstr_fd("Malloc error creating key for run_assign_str", 2), 1);
	if (equal + 1 == '\0')
	{
		value = ft_strdup("");
		if (!value)
			return (ft_putstr_fd("Malloc error creating value for run_assign_str", 2), 1);
	}
	else
	{
		value = strdup_range(equal + 1, cmd + ft_strlen(cmd));
		if (!value)
			return (ft_putstr_fd("Malloc error creating value for run_assign_str", 2), 1);
	}
	exist = check_exist(key, shell->var_list);
	if (exist)
	{
		free(exist->value);
		exist->value = value;
		if (equal)
	}
	else if (valid_varname(key) == 1)
	{
		if (create_node(&shell->var_list, key) == 0) // note: create_node returns 0 for failure
		{
			//error message: malloc error
			return (1);
		}
		free(key);
	}
	else
	{
		//error message: key is not a valid identifier
		return (1);
	}
}

/*	run_assign runs the assignments in cmd and args (if any) 
	- returns 1 if failure (malloc) */
// work in progress - need to combine cmd and arg to run at the same time 
// if arg contains invalid varname, cmd assignment doesn't run 
int	run_assign(t_ast *node, t_shell *shell)
{
	int		i;
	
	// check all varnames before running
	if (run_assign_str(node->cmd, shell) == 1)
		return (1);
	if (node->n_args > 0)
	{
		i = 0;
		while (i < node->n_args)
		{
			if (run_assign_str(node->args[i]) == 1)
				return (1);
			i++;
		}
	}
	return (0);
}
