/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/06 10:48:52 by mateo            ###   ########.fr       */
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
		- if key already exists (as env or normal), 
			- if no equal, change to env (if originally normal); original value is retained
			- if equal, replace in list
		- if key doesn't exist, create new node in list 
	- options are treated as invalid variable names
	*/
	//work in progress: check_exist not updated since it is used for expansions too 
// work in progress: streamline function by using temp t_var to hold key + value and another function to create them (function will be used in run_assign_str)

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
			if (create_key_value(node->args[i], &equal, &key, &value) == 1)
				return (1);
			if (valid_varname(key) == 1)
			{
				free(key);
				free(value);
				return (ft_putstr_fd("export: invalid environment variable name\n", 2), 1);
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
				free(key);
				free(value);
				if (equal)
				{
					if (create_node(shell->var_list, argv[i], 1) == 1)
						return (1); // update this

				}
				else
				{
					if (create_node(shell->var_list, argv[i], 0) == 1)
						return (1); // update this 

				}
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

