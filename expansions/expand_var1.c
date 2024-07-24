/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 15:10:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	file_list_check_var calls contain_var and expand_str
	for each file provided in file linked list
	*/
int	file_list_check_var(t_file *file, t_shell *shell)
{
	t_file	*curr_file;

	curr_file = file;
	while (curr_file)
	{
		if (contain_var(curr_file->file_name) == 0)
		{
			curr_file->file_name = expand_str(curr_file->file_name, shell);
			if (!curr_file->file_name)
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}

/*	check_var_expand_arg checks for var expansion in args
	- returns 1 if error */
int	check_var_expand_arg(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	curr_arg = node->args;
	while (curr_arg)
	{
		if (contain_var(curr_arg->content) == 0)
		{
			curr_arg->content = expand_str(curr_arg->content, shell);
			if (!curr_arg->content)
				return (1);
		}
		curr_arg = curr_arg->next;
	}
	return (0);
}

/*	check_var_expansion checks whether variable expansions are needed
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion (terminate shell)
	*/
int	check_var_expansion(t_ast *node, t_shell *shell)
{
	if (contain_var(node->cmd) == 0)
	{
		node->cmd = expand_str(node->cmd, shell);
		if (!node->cmd)
			return (1);
	}
	if (node->n_args > 0)
		if (check_var_expand_arg(node, shell) == 1)
			return (1);
	if (node->input_list)
		if (file_list_check_var(node->input_list, shell) == 1)
			return (1);
	if (node->heredoc_list)
		if (file_list_check_var(node->heredoc_list, shell) == 1)
			return (1);
	if (node->output_list)
		if (file_list_check_var(node->output_list, shell) == 1)
			return (1);
	return (0);
}
