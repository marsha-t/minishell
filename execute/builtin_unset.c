/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 06:17:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/07 16:35:10 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_unset runs the unset command
	- if no args, unset returns with success immediately
	- if invalid variable name, skip to next variable but eventually return with error 
	- if variable not found, skip to next variable 
	- else if variable found, remove node from list
	- unset works for environment and normal vars
	- options are treated as invalid variable names
	*/
int	builtin_unset(t_ast *node, t_shell *shell)
{
	t_var	*current;
	t_var	*del_node;
	int		exit_status;
	t_list	*curr_arg;
	
	exit_status = 0;
	if (node->n_args == 0)
		return (0);
	curr_arg = node->args;
	while (curr_arg)
	{
		if (valid_varname(curr_arg->content) == 1)
		{
			ft_putstr_fd("minishell: unset: invalid environment variable name\n", 2);
			exit_status = 1;
		}
		else if (check_exist(curr_arg->content, shell->var_list))
		{
			del_node = check_exist(curr_arg->content, shell->var_list);
			if (del_node == shell->var_list)
			{
				shell->var_list = shell->var_list->next;
				free_num(3, del_node->key, del_node->value, del_node);
			}
			else
			{
				current = shell->var_list;
				while (current->next)
				{
					if (current->next == del_node)
					{
						current->next = del_node->next;
						free_num(3, del_node->key, del_node->value, del_node);
						break;
					}
					current = current->next;
				}
			}
		}
		curr_arg = curr_arg->next;
	}
	return (exit_status);
}
