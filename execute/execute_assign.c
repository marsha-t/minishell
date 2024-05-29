/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/05/29 08:34:06 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/*	run_assign */
// work in progress
int	run_assign(t_ast *node, t_shell *shell)
{
	int		equal;
	char	*varname;
	char	*definition;
	
	run_assign_cmd(node->cmd);
	run_assign_arg(node->cmd);
	equal = ft_strchr(node->cmd, '=');
	varname = strdup_range(node->cmd, equal - 1]);
	definition = strdup_range(equal + 1, strdup_range(node->cmd[ft_strlen(node->cmd) - 1]));
	if (!varname)
	{
		// error message: malloc error
		return (1);
	}
	if (/*in var list*/)
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
	return (0);
}
