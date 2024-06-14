/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/14 15:41:05 by mateo            ###   ########.fr       */
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
// work in progress: check_exist not updated since it is used for expansions too 
// work in progress: to terminate shell for malloc issues
// work in progress: update to use in_fd and out_fd
int builtin_export(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	t_list	*curr_arg;
	char	*key;
	char	*value;
	char	*equal;
	t_var	*exist;

	(void)in_fd;
	(void)out_fd;
	if (node->n_args == 0)
		return (print_export(shell->var_list), 0);
	else
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (create_key_value(curr_arg->content, &equal, &key, &value) == 1)
				return (1); // terminate shell
			if (valid_varname(key) == 1)
			{
				free_num(2, key, value);
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
				free_num(2, key, value);
				if (equal)
				{
					if (create_node(&shell->var_list, curr_arg->content, 1) == 1)
						return (1); // need to terminate shell
				}
				else
				{
					if (create_node(&shell->var_list, curr_arg->content, 0) == 1)
						return (1); // need to terminate shell
				}
			}
			curr_arg = curr_arg->next;
		}
	}
	return (0);
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

