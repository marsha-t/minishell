/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/06/05 07:07:48 by mateo            ###   ########.fr       */
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
	- if variable name already exists, replaces definition 
	- else checks if variable name is valid 
		if yes, new variable added to end of linked list
		if not, error msg */
// work in progress pending structure of variable linked list and shell
int	run_assign_str(char *cmd, t_shell *shell)
{
	int		equal;
	char	*varname;
	char	*definition;
	
	equal = ft_strchr(cmd, '=');
	varname = strdup_range(cmd, equal - 1);
	if (!varname)
	{
		// error message: malloc error
		return (1);
	}
	definition = strdup_range(equal + 1, cmd + ft_strlen(cmd));
	if (!definition)
	{
		// error message: malloc error
		return (1);
	}
	if (/*varname in var list*/)
	{
		// replace in list 
	}
	else if (valid_varname(varname) == 1)
	{
		if (create_node(&shell->var_list, varname) == 0) // note: create_node returns 0 for failure
		{
			//error message: malloc error
			return (1);
		}
		free(varname);
	}
	else
	{
		//error message: varname is not a valid identifier
		return (1);
	}
}

/*	run_assign runs the assignments in cmd and args (if any) 
	- returns 1 if failure (malloc) */
int	run_assign(t_ast *node, t_shell *shell)
{
	int		i;
	
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
