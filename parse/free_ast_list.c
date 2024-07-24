/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:10:49 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 18:12:31 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	file_list_free frees nodes in the files linked list
	and the file names stored in each node */
void	file_list_free(t_file *file)
{
	t_file	*current;
	t_file	*next;

	current = file;
	while (current)
	{
		next = current->next;
		free(current->file_name);
		free(current);
		current = next;
	}
}

/*	arg_list_free frees nodes in the args linked list
	and the content stored in each node */
void	arg_list_free(t_ast *current)
{
	t_list	*curr_arg;

	curr_arg = current->args;
	while (curr_arg)
	{
		current->args = current->args->next;
		free(curr_arg->content);
		free(curr_arg);
		curr_arg = current->args;
	}
}

/*	ast_list_free frees nodes in ast linked list, cmd,
	args and its component strings, input, output, append 
*/
void	ast_list_free(t_ast *node)
{
	t_ast	*current;
	t_ast	*next;

	current = node;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->args)
			arg_list_free(current);
		if (current->input_list)
			file_list_free(current->input_list);
		if (current->heredoc_list)
			file_list_free(current->heredoc_list);
		if (current->output_list)
			file_list_free(current->output_list);
		free(current);
		current = next;
	}
	node = NULL;
}
