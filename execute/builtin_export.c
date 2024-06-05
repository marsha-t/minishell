/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/05 07:15:27 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_export runs the export command
	- if no args, prints environment variable list 
	- if there are args, 
		- checks whether there is an equal sign
			- if there is no equal sign, variable is listed in export but not in env
		- checks whether key is a valid variable name
		- if key is invalid, returns error
		- if key already exists, then replace in list
		- if key doesn't exist, create new node in list 
	*/
	//work in progress: check_exist not updated since it is used for expansions too 
int builtin_export(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	int	i;
	char	*key;
	char	*value;
	char	*equal;
	t_var	*exist;

	if (node->n_args == 0)
		return (print_export(shell->var_list));
	else
	{
		while (i < node->n_args)
		{
			equal = ft_strchr(node->args[i], '=');
			if (equal)
			{
				key = strdup_range(node->args[i], equal - 1);
				if (!key)
					return (ft_putstr_fd("Malloc error creating key in builtin_export\n", 2), 1);
				value = strdup_range(equal + 1, node->args[i] + ft_strlen(node->args[i]) - 1);
				if (!value)
					return (ft_putstr_fd("Malloc error creating value in builtin_export\n", 2), 1);
			}
			else
			{
				key = ft_strdup(node->args[i]);
				if (!key)
					return (ft_putstr_fd("Malloc error creating key in builtin_export\n", 2), 1);
				value = 0;
			}
			if (valid_varname(key) == 0)
			{
				free(key);
				free(value);
				return (ft_putstr_fd("export: invalid environment variable name\n", 2), 1);
			}
			exist = check_exist(key, shell->var_list);
			if (exist)
			{
				free(exist->value);
				exist->value = value;
				if (equal)
					exist->flag = 1;
				else
					exist->flag = 0;
				exist->env = 1;
				free(key);
			}
			else
			{
				free(key);
				free(value);
				if (equal)
					create_node(shell->var_list, argv[i], 1);
				else
					create_node(shell->var_list, argv[i], 0);
			}
			i++;
		}
	}
}


// void export(char *s1,char *s2, t_var **envp)
// {
// 	t_var *current;
// 	// t_var *p;
// 	t_var *new;

// 	current = *envp;
// 	new = malloc(sizeof(t_var));
// 	if(ft_strcmp(s1, "export") == 0 && s2 )
// 	{
// 		if(check_exist(s2,*envp))
// 		{
// 			new= check_exist(s2, *envp);
// 			new ->content = s2;
// 		}
// 		else
// 		{
// 			*envp = new;
// 			new -> next = current;
// 			new ->content = s2;
// 			if(!(ft_strchr(s2,'=')))
// 				new-> flag = 1;
// 		}
// 	}
// }

