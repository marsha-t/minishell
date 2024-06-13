/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 06:17:45 by mateo             #+#    #+#             */
/*   Updated: 2024/06/13 13:44:30 by mateo            ###   ########.fr       */
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
// work in progress: need to update search_for_node (which uses search_for_key which expansions use)
int	builtin_unset(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	t_var	*current;
	t_var	*del_node;
	int		exit_status;
	t_file	*curr_arg;
	
	exit_status = 0;
	if (node->n_args == 0)
		return (0);
	curr_arg = node->args;
	while (curr_arg)
	{
		if (valid_varname(curr_arg->content) == 1)
		{
			ft_putstr_fd("unset: invalid environment variable name\n", 2);
			exit_status = 1;
		}
		else if (search_for_node(curr_arg->content, shell->var_list))
		{
			del_node = search_for_node(curr_arg->content, shell->var_list);
			if (del_node == shell->var_list)
			{
				shell->var_list = shell->var_list->next;
				free_num(3, del_node->key, del_node->value, del_node);
			}
			current = shell->var_list;
			while (current->next)
			{
				if (current->next== del_node)
				{
					current->next = del_node->next;
					free_num(3, del_node->key, del_node->value, del_node);
				}
				current = current-> next;
			}
		}
		curr_arg = curr_arg->next;
	}
	return (0);
}

// void unset(char *s1,char *s2, t_var **envp)
// {
// 	t_var *current;
// 	current = *envp;
// 	t_var *del_node;
// 	printf("%d \n",ft_strcmp(s1, "unset"));
// 	del_node = malloc(sizeof(t_var));
// 	if(ft_strcmp(s1, "unset")== 0)
// 	{
// 		if(search_for_node(s2, envp) )
// 			del_node = search_for_node(s2, envp);
// 		else
// 			return ;
// 	}
// 	if (del_node == *envp)
// 		return((*envp)= (*envp)->next ,free(del_node)) ;

// 	while(current->next)
// 	{
// 		if(current-> next== del_node)
// 		{
// 			current ->next = del_node ->next;
// 			free(del_node);
// 			return ;
// 		}
// 		current = current-> next;
// 	}
// }
